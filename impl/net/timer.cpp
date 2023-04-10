//
// Created by henry on 4/6/23.
//

#include "../../include/hagame/net/timer.h"

void hg::net::Timer::wait(long seconds) {
    asio::steady_timer t(io, asio::chrono::seconds(seconds));
    t.wait();
}
