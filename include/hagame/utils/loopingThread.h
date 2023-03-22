//
// Created by henry on 2/3/23.
//

#ifndef HAGAME2_LOOPINGTHREAD_H
#define HAGAME2_LOOPINGTHREAD_H

#include <thread>

namespace hg::utils {
class LoopingThread {
    public:

        LoopingThread() {}
        ~LoopingThread() {
            abort();
        }

        bool run() {
            try {
                m_thread = std::thread(&LoopingThread::tickFunc, this);
            } catch(...) {
                return false;
            }
            return true;
        }

        void stop() {
            abort();
        }

    protected:

        virtual void onStart() {}
        virtual void onTick() {}
        virtual void onAbort() {}

    private:

        std::atomic_bool m_aborting;
        std::atomic_bool m_running;
        std::thread m_thread;

        void tickFunc() {
            m_running.store(true);

            onStart();

            while(true)
            {
                try
                {
                    onTick();
                }
                catch(std::runtime_error& e)
                {
                    std::cout << e.what() << "\n";
                    // Some more specific
                }
                catch(...)
                {
                    std::cout << "Failed for unknown reason\n";
                    // Make sure that nothing leaves the thread for now...
                }
            }

            m_running.store(false);
        }

        void abort() {
            m_aborting.store(true);
            if (m_thread.joinable()) {
                onAbort();
                m_thread.join();
            }
        }
    };
}

#endif //HAGAME2_LOOPINGTHREAD_H
