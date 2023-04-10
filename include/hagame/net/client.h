//
// Created by henry on 4/8/23.
//

#ifndef HAGAME2_CLIENT_H
#define HAGAME2_CLIENT_H

#include "net.h"
#include "tsqueue.h"
#include "message.h"

namespace hg::net {
    template<typename T>
    class Client {
    public:

        Client() {}
        ~Client() {
            disconnect();
        }

        bool connect(const std::string& host, const uint16_t port) {
            try {
                asio::ip::tcp::resolver resolver(m_context);
                asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));
                m_connection = std::make_unique<Connection<T>>(Connection<T>::Owner::Client, m_context, asio::ip::tcp::socket(m_context), m_queueIn);
                m_connection->connectToServer(endpoints);
                m_thread = std::thread([this](){m_context.run();});
            } catch (std::exception& e) {
                std::cerr << "[CLIENT] Exception: " << e.what() << "\n";
                return false;
            }
            return true;
        }

        void disconnect() {
            if (isConnected()) {
                m_connection->disconnect();
            }

            m_context.stop();

            if (m_thread.joinable()) {
                m_thread.join();
            }

            m_connection.release();
        }

        bool isConnected() {
            if (m_connection) {
                return m_connection->isConnected();
            } else {
                return false;
            }
        }

        void send(const Message<T>& msg) {
            if (isConnected()) {
                m_connection->send(msg);
            }
        }

        TSQueue<OwnedMessage<T>>& incoming() {
            return m_queueIn;
        }

    protected:

        asio::io_context m_context;
        std::thread m_thread;

        std::unique_ptr<Connection<T>> m_connection;

    private:

        TSQueue<OwnedMessage<T>> m_queueIn;

    };
}

#endif //HAGAME2_CLIENT_H
