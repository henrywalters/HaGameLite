//
// Created by henry on 12/14/22.
//

#ifndef HAGAME2_CLOCK_H
#define HAGAME2_CLOCK_H

#include <chrono>

namespace hg::utils {
    class Clock {
    public:
        static long long Now() {
            auto time = std::chrono::high_resolution_clock::now();
            return std::chrono::duration_cast<std::chrono::nanoseconds>(time.time_since_epoch()).count();
        }

        static void Wait(long microseconds) {
            auto elapsed = 0;
            auto start = Clock::Now();
            while (elapsed < microseconds) {
                elapsed = Clock::Now() - start;
            }
        }

        static double ToSeconds(long duration) {
            return ((double)duration / (double) std::chrono::nanoseconds::period::den);
        }
    };
}


#endif //HAGAME2_CLOCK_H
