//
// Created by henry on 4/7/23.
//

#ifndef HAGAME2_CONNECTION_H
#define HAGAME2_CONNECTION_H

#include "net.h"

#include "message.h"
#include "tsqueue.h"
#include "../utils/uuid.h"

namespace hg::net {
    template <typename T>
    class Server;

    template <typename T>
    class Connection : public std::enable_shared_from_this<Connection<T>> {
    public:

        enum class Owner {
            Server,
            Client,
        };

        Connection(Owner parent, asio::io_context& context, asio::ip::tcp::socket socket, TSQueue<OwnedMessage<T>>& queueIn):
            m_context(context),
            m_socket(std::move(socket)),
            m_queueIn(queueIn),
            m_ownerType(parent)
        {
            if (m_ownerType == Owner::Server) {
                m_handshakeOut = uint64_t(std::chrono::system_clock::now().time_since_epoch().count());
                m_handshakeCheck = scramble(m_handshakeOut);
            }
        }

        utils::uuid_t getID() const {
            return m_id;
        }

        void connectToClient(Server<T>* server, utils::uuid_t uid = 0) {
            if (m_ownerType == Owner::Server) {
                if (m_socket.is_open()) {
                    m_id = uid;
                    writeValidation();
                    readValidation(server);
                }
            }
        }

        void connectToServer(const asio::ip::tcp::resolver::results_type& endpoints) {
            if (m_ownerType == Owner::Client) {
                asio::async_connect(m_socket, endpoints, [this](std::error_code err, asio::ip::tcp::endpoint endpoint) {
                    if (!err) {
                        readValidation();
                    }
                });
            }
        }

        void disconnect() {
            if (isConnected()) {
                asio::post(m_context, [this](){ m_socket.close(); });
            }
        }

        bool isConnected() const {
            return m_socket.is_open();
        }

        void send(const Message<T>& msg) {
            asio::post(m_context, [this, msg](){
               bool writing = !m_queueOut.empty();
               m_queueOut.push_back(msg);
               if (!writing) {
                   writeHeader();
               }
            });
        }

    protected:

        asio::ip::tcp::socket m_socket;
        asio::io_context& m_context;
        TSQueue<Message<T>> m_queueOut;
        TSQueue<OwnedMessage<T>>& m_queueIn;

        Message<T> m_msgTmpIn;

        Owner m_ownerType = Owner::Server;

        uint64_t m_handshakeOut = 0;
        uint64_t m_handshakeIn = 0;
        uint64_t m_handshakeCheck = 0;

        bool m_validHandshake = false;
        bool m_connectionEstablished = false;

        utils::uuid_t m_id = 0;

    private:

        void writeHeader() {
            asio::async_write(m_socket, asio::buffer(&m_queueOut.front().header, sizeof(MessageHeader<T>)), [this](std::error_code err, std::size_t length) {
               if (!err) {
                   if (m_queueOut.front().body.size() > 0) {
                       writeBody();
                   } else {
                       m_queueOut.pop_front();

                       if (!m_queueOut.empty()) {
                           writeHeader();
                       }
                   }
               } else {
                   std::cout << "[" << m_id << "] WRITE HEADER FAILED\n";
                   m_socket.close();
               }
            });
        }

        void writeBody() {
            asio::async_write(m_socket, asio::buffer(m_queueOut.front().body.data(), m_queueOut.front().body.size()), [this](std::error_code err, std::size_t length) {
                if (!err) {
                    m_queueOut.pop_front();

                    if (!m_queueOut.empty()) {
                        writeHeader();
                    }
                } else {
                    std::cout << "[" << m_id << "] WRITE BODY FAILED\n";
                    m_socket.close();
                }
            });
        }

        void readHeader() {
            asio::async_read(m_socket, asio::buffer(&m_msgTmpIn.header, sizeof(MessageHeader<T>)), [this](std::error_code err, std::size_t length) {
                if (!err) {
                    if (m_msgTmpIn.header.size > 0) {
                        m_msgTmpIn.body.resize(m_msgTmpIn.header.size);
                        readBody();
                    } else {
                        addToIncomingMessageQueue();
                    }
                } else {
                    std::cout << "[" << m_id << "] READ HEADER FAILED: " << err.message() << "\n";
                    m_socket.close();
                }
            });
        }

        void readBody() {
            asio::async_read(m_socket, asio::buffer(m_msgTmpIn.body.data(), m_msgTmpIn.body.size()), [this](std::error_code err, std::size_t length) {
                if (!err) {
                    addToIncomingMessageQueue();
                } else {
                    std::cout << "[" << m_id << "] READ BODY FAILED\n";
                    m_socket.close();
                }
            });
        }

        uint64_t scramble(uint64_t input) {
            uint64_t out = input ^ 0xDEADBEEFC0DECAFE;
            out = (out & 0xF0F0F0F0F0F0F0) >> 4 | (out & 0x0F0F0F0F0F0F0F) << 4;
            return out ^ 0xC0DEFACE12345678;
        }

        void writeValidation() {
            asio::async_write(m_socket, asio::buffer(&m_handshakeOut, sizeof(uint64_t)), [this](std::error_code err, std::size_t length) {
                if (!err) {
                    if (m_ownerType == Owner::Client) {
                        readHeader();
                    }
                } else {
                    std::cout << "[" << m_id << "] WRITE VALIDATION FAILED\n";
                    m_socket.close();
                }
            });
        }

        void readValidation(Server<T>* server = nullptr) {
            asio::async_read(m_socket, asio::buffer(&m_handshakeIn, sizeof(uint64_t)), [this, server](std::error_code err, std::size_t length) {
                if (!err) {
                    if (m_ownerType == Owner::Server) {
                        if (m_handshakeIn == m_handshakeCheck) {
                            std::cout << "CLIENT VALIDATED\n";
                            server->onClientValidated(this);
                            readHeader();
                        } else {
                            std::cout << "[" << m_id << "] FAILED TO VALIDATE\n";
                            m_socket.close();
                        }
                    } else {
                        m_handshakeOut = scramble(m_handshakeIn);
                        writeValidation();
                    }
                } else {
                    std::cout << "[" << m_id << "] FAILED TO READ VALIDATION\n";
                    m_socket.close();
                }
            });
        }

        void addToIncomingMessageQueue() {
            if (m_ownerType == Owner::Server) {
                m_queueIn.push_back({this->shared_from_this(), m_msgTmpIn});
            } else {
                m_queueIn.push_back({nullptr, m_msgTmpIn});
            }

            readHeader();
        }

    };
}

#endif //HAGAME2_CONNECTION_H
