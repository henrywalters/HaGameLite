//
// Created by henry on 12/28/22.
//

#ifndef HAGAME2_RANDOM_H
#define HAGAME2_RANDOM_H

#include <random>

namespace hg::utils {
    class Random {
        std::random_device device;
        std::mt19937 generator;

    public:

        Random() : generator(device()) {}

        template<class T = float>
        T real(T min, T max) {
            std::uniform_real_distribution<T> dist(min, max);
            return dist(generator);
        }

        template<class T = int>
        T integer(T min, T max) {
            std::uniform_int_distribution<T> dist(min, max);
            return dist(generator);
        }

        template<class T = float>
        T normal(T mean, T stdev) {
            std::normal_distribution<T> dist(mean, stdev);
            return dist(generator);
        }

        template<class T = float>
        T poisson(T mean) {
            std::poisson_distribution<T> dist(mean);
            return dist(generator);
        }

        template<class T = float>
        T lognorm(T mean, T stddev) {
            std::lognormal_distribution dist(mean, stddev);
            return dist(generator);
        }
    };
}

#endif //HAGAME2_RANDOM_H
