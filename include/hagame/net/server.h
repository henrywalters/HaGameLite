//
// Created by henry on 4/7/23.
//

#ifndef HAGAME2_SERVER_H
#define HAGAME2_SERVER_H

#include "net.h"
#include "../structures/tsqueue.h"
#include "connection.h"
#include "../utils/uuid.h"

namespace hg::net {
    template <typename T>
    class Server {
    public:

        Server(uint16_t port):
                m_acceptor(m_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
        {
            std::cout << "[SERVER] LISTENING ON PORT " << port << "\n";
        }

        virtual ~Server() {
            stop();
        }

        bool start() {
            try {
                waitForClientConnection();
                m_thread = std::thread([this]() {m_context.run(); });
                return true;
            } catch(std::exception& e) {
                std::cerr << "[SERVER] Exception: " << e.what() << "\n";
                return false;
            }
        }

        void stop() {
            m_context.stop();
            if (m_thread.joinable()) {
                m_thread.join();
            }
            std::cout << "[SERVER] STOPPED\n";
        }

        void waitForClientConnection() {
            m_acceptor.async_accept([this](std::error_code err, asio::ip::tcp::socket socket) {
                if (!err) {
                    std::cout << "[SERVER] NEW CONNECTION: " << socket.remote_endpoint() << "\n";
                    std::shared_ptr<Connection<T>> conn = std::make_shared<Connection<T>>(Connection<T>::Owner::Server, m_context, std::move(socket), m_queueIn);

                    if (onClientConnect(conn)) {
                        m_connections.push_back(std::move(conn));
                        m_connections.back()->connectToClient(this, m_idCounter++);
                        std::cout << "[" << m_connections.back()->getID() << "] CONNECTION APPROVED\n";
                    } else {
                        std::cout << "[SERVER] CONNECTION DENIED\n";
                    }
                } else {
                    std::cout << "[SERVER] Connection Error: " << err.message() << "\n";
                }

                waitForClientConnection();
            });
        }

        void messageClient(std::shared_ptr<Connection<T>> client, const Message<T>& msg) {
            if (client && client->isConnected()) {
                client->send(msg);
            } else {
                onClientDisconnect(client);
                client.reset();
                m_connections.erase(std::remove(m_connections.begin(), m_connections.end(), client), m_connections.end());
            }
        }

        void messageAllClients(const Message<T>& msg) {

            bool cleanupClients = false;

            for (auto& client : m_connections) {
                if (client && client->isConnected()) {
                    client->send(msg);
                } else {
                    onClientDisconnect(client);
                    client.reset();
                    cleanupClients = true;
                }
            }

            if (cleanupClients) {
                m_connections.erase(std::remove(m_connections.begin(), m_connections.end(), nullptr), m_connections.end());
            }
        }

        void update(bool wait) {
            if (wait) {
                m_queueIn.wait();
            }

            while (!m_queueIn.empty()) {
                auto msg = m_queueIn.pop_front();
                onMessage(msg.remote, msg.msg);
            }
        }

        virtual void onClientValidated(Connection<T>* client) {}

    protected:

        TSQueue<OwnedMessage<T>> m_queueIn;

        std::deque<std::shared_ptr<Connection<T>>> m_connections;

        asio::io_context m_context;
        std::thread m_thread;

        asio::ip::tcp::acceptor m_acceptor;

        utils::uuid_t m_idCounter = 10000;

        virtual bool onClientConnect(std::shared_ptr<Connection<T>> client) {
            return false;
        }

        virtual void onClientDisconnect(std::shared_ptr<Connection<T>> client) {}

        virtual void onMessage(std::shared_ptr<Connection<T>> client, Message<T>& msg) {}

    };
}

#endif //HAGAME2_SERVER_H
