//
// Created by henry on 12/14/22.
//

#ifndef HAGAME2_TIMER_H
#define HAGAME2_TIMER_H

#include "./clock.h"


namespace hg::utils {

    // Record the amount of time that has passed
    class Timer {
    public:
        long _start;

        Timer(): _start(Clock::Now()) {}

        void reset() {
            _start = Clock::Now();
        }

        long elapsed() {
            return Clock::Now() - _start;
        }

    };
}


#endif //HAGAME2_TIMER_H
