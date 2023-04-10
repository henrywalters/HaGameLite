//
// Created by henry on 4/6/23.
//

#ifndef HAGAME2_TIMER_H
#define HAGAME2_TIMER_H

#include <asio.hpp>

namespace hg::net {
    class Timer {
    public:
        void wait(long seconds);
    private:
        asio::io_context io;
    };
}

#endif //HAGAME2_TIMER_H
