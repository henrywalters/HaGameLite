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
                if (point[i] < pos[i] || point[i] > pos[i] + size[i]) return false;
            }
            return true;
        }

        bool intersects(Hypercube<dimensions, T> cube) {
            auto a = getCenter();
            auto b = cube.getCenter();
            for (int i = 0; i < dimensions; i++) {
                if (std::abs(a[i] - b[i]) > (size[i] + cube.size[i]) / 2) return false;
            }
            return true;
        }

        std::string toString() const {
            return "Position: " + pos.toString() + " Size: " + size.toString();
        }

        static Hypercube Zero() {
            return Hypercube(Vector<dimensions, T>::Zero(), Vector<dimensions, T>::Zero());
        }

        Vector<dimensions, T> getCenter() {
            return pos + (size * 0.5);
        }

        Vector<dimensions, T> closestPoint(Vector<dimensions, T> point) {
            Vector<dimensions, T> out;
            auto center = getCenter();
            for (int i = 0; i < dimensions; i++) {
                float v = point[i];
                float m = center[i] - size[i] / 2;
                float M = center[i] + size[i] / 2;
                if (v < m) v = m;
                if (v > M) v = M;
                out[i] = v;
            }
            return out;
        }

        Vector<dimensions, T> min() {
            return pos;
        }

        Vector<dimensions, T> max() {
            return pos + size;
        }

        friend std::ostream& operator <<(std::ostream& out, const Hypercube& cube) {
            out << cube.toString();
            return out;
        }

        operator std::string() const {
            std::ostringstream out;
            out << *this;
            return out.str();
        }


    };
}

#endif //HAGAME2_HYPERCUBE_H
