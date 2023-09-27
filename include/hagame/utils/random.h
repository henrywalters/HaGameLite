//
// Created by henry on 12/28/22.
//

#ifndef HAGAME2_RANDOM_H
#define HAGAME2_RANDOM_H

#include <random>
#include "../math/vector.h"

namespace hg::utils {
    class Random {
        std::random_device device;
        std::mt19937 generator;

    public:

        Random() : generator(device()) {}
        Random(int seed): generator(seed) {}

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

        template <size_t N>
        hg::math::Vector<N, int> integerVector(int min, int max) {
            hg::math::Vector<N, int> vec;
            for (int i = 0; i < N; i++) {
                vec[i] = integer(min, max);
            }
            return vec;
        }

        template <size_t N, class T = float>
        hg::math::Vector<N, T> realVector(T min, T max) {
            hg::math::Vector<N, int> vec;
            for (int i = 0; i < N; i++) {
                vec[i] = real<T>(min, max);
            }
            return vec;
        }
    };
}

#endif //HAGAME2_RANDOM_H
