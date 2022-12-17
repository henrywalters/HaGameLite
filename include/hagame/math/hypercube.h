//
// Created by henry on 12/16/22.
//

#ifndef HAGAME2_HYPERCUBE_H
#define HAGAME2_HYPERCUBE_H

#include "vector.h"

namespace hg::math {
    // A base cube class of n-dimensions.
    template <size_t dimensions, class T>
    class Hypercube {
    public:
        Vector<dimensions, T> pos;
        Vector<dimensions, T> size;

        Hypercube() {
            pos = Vector<dimensions, T>();
            size = Vector<dimensions, T>();
        }

        Hypercube(Vector<dimensions, T> _position, Vector<dimensions, T> _size) {
            pos = _position;
            size = _size;
        }

        bool contains(Vector<dimensions, T> point) {
            for (int i = 0; i < dimensions; i++) {
                if (point[i] < pos[i] || point[i] >= pos[i] + size[i]) return false;
            }
            return true;
        }

        std::string toString() {
            return "Position: " + pos.toString() + " Size: " + size.toString();
        }

        static Hypercube Zero() {
            return Hypercube(Vector<dimensions, T>::Zero(), Vector<dimensions, T>::Zero());
        }

        Vector<dimensions, T> getCenter() {
            return pos + (size * 0.5);
        }

        Vector<dimensions, T> min() {
            return pos;
        }

        Vector<dimensions, T> max() {
            return pos + size;
        }

    };
}

#endif //HAGAME2_HYPERCUBE_H
