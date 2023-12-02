//
// Created by henry on 4/7/23.
//

#ifndef HAGAME2_TSQUEUE_H
#define HAGAME2_TSQUEUE_H

#include <mutex>
#include <deque>
#include <condition_variable>

namespace hg::structures {
    // TSQueue provides a thread-safe queue for passing buffers to a separate thread
    template <typename T>
    class TSQueue {
    public:
        TSQueue() = default;
        TSQueue(const TSQueue<T>&) = delete;
        virtual ~TSQueue() { clear(); }

        const T& front() {
            std::scoped_lock lock(m_mutex);
            return m_queue.front();
        }

        const T& back() {
            std::scoped_lock lock(m_mutex);
            return m_queue.back();
        }

        T pop_front() {
            std::scoped_lock lock(m_mutex);
            auto t = std::move(m_queue.front());
            m_queue.pop_front();
            return t;
        }

        T pop_back() {
            std::scoped_lock lock(m_mutex);
            auto t = std::move(m_queue.back());
            m_queue.pop_back();
            return t;
        }

        void push_back(const T& item) {
            std::scoped_lock lock(m_mutex);
            m_queue.emplace_back(std::move(item));
            std::unique_lock<std::mutex> ul(m_blockingMutex);
            m_blocking.notify_one();
        }

        void push_front(const T& item) {
            std::scoped_lock lock(m_mutex);
            m_queue.emplace_front(std::move(item));
            std::unique_lock<std::mutex> ul(m_blockingMutex);
            m_blocking.notify_one();
        }

        bool empty() {
            std::scoped_lock lock(m_mutex);
            return m_queue.empty();
        }

        size_t size() {
            std::scoped_lock lock(m_mutex);
            return m_queue.size();
        }

        void clear() {
            std::scoped_lock lock(m_mutex);
            m_queue.clear();
        }

        void wait() {
            while (empty()) {
                std::unique_lock<std::mutex> ul(m_blockingMutex);
                m_blocking.wait(ul);
            }
        }

    protected:

        std::mutex m_mutex;
        std::deque<T> m_queue;
        std::condition_variable m_blocking;
        std::mutex m_blockingMutex;

    };
}

#endif //HAGAME2_TSQUEUE_H
