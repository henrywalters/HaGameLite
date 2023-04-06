//
// Created by henry on 12/12/22.
//

#ifndef HAGAME2_VECTOR_H
#define HAGAME2_VECTOR_H

#include <functional>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>

#include "constants.h"

namespace hg::math {

        template <class T>
        class Quaternion;

        // Generic vector class that can be extended to fit any needs.
        template<size_t size, class T>
        class Vector {
        public:

            T vector[size];

            Vector(std::vector<T> vect) {
                for (int i = 0; i < size; i++) {
                    vector[i] = i < vect.size() ? vect[i] : 0;
                }
            }

            Vector() {
                for (int i = 0; i < size; i++) {
                    vector[i] = 0;
                }
            }

            Vector(T val) {
                for (int i = 0; i < size; i++) {
                    vector[i] = val;
                }
            }

            Vector(const Vector& vect) {
                for (int i = 0; i < size; i++) {
                    vector[i] = vect[i];
                }
            }

            Vector(T vect[size]) {
                for (int i = 0; i < size; i++) {
                    vector[i] = vect[i];
                }
            }

            Vector(T x, T y) {
                vector[0] = x;
                vector[1] = y;
            }

            Vector(T x, T y, T z) {
                vector[0] = x;
                vector[1] = y;
                vector[2] = z;
            }

            // Static Helpers

            static constexpr Vector Identity() {
                Vector vect;
                for (int i = 0; i < size; i++) {
                    vect[i] = 1;
                }
                return vect;
            }

            static constexpr Vector Zero() {
                Vector vect;
                return vect;
            }

            static constexpr Vector<3, T> Right() {
                return Vector(1, 0, 0);
            }

            static constexpr Vector<3, T> Left() {
                return Vector(-1, 0, 0);
            }

            static constexpr Vector<3, T> Top() {
                return Vector(0, 1, 0);
            }

            static constexpr Vector<3, T> Bottom() {
                return Vector(0, -1, 0);
            }

            static constexpr Vector<3, T> Face() {
                return Vector(0, 0, 1);
            }

            static constexpr Vector<3, T> Back() {
                return Vector(0, 0, -1);
            }

            static constexpr Vector<3, T> Right(T value) {
                return Vector(value, 0, 0);
            }

            static constexpr Vector<3, T> Left(T value) {
                return Vector(-value, 0, 0);
            }

            static constexpr Vector<3, T> Top(T value) {
                return Vector(0, value, 0);
            }

            static constexpr Vector<3, T> Bottom(T value) {
                return Vector(0, -value, 0);
            }

            static constexpr Vector<3, T> Face(T value) {
                return Vector(0, 0, value);
            }

            static constexpr Vector<3, T> Back(T value) {
                return Vector(0, 0, -value);
            }

            // Basic accessors
            constexpr T x() {
                return vector[0];
            }

            constexpr T y() {
                return vector[1];
            }

            constexpr T z() {
                return vector[2];
            }

            constexpr Vector<2, T> xy() {
                return Vector<2, T>(vector[0], vector[1]);
            }

            constexpr Vector<2, T> xz() {
                return Vector<2, T>(vector[0], vector[2]);
            }

            constexpr Vector<2, T> yz() {
                return Vector<2, T>(vector[1], vector[2]);
            }

            constexpr Vector<3, T> xyz() {
                return Vector<3, T>(vector[0], vector[1], vector[2]);
            }

            // Basic functions

            // Set all elements to zero

            Vector copy() const {
                Vector copy = Vector();
                for (int i = 0; i < size; i++) {
                    copy[i] = vector[i];
                }
                return copy;
            }

            // In-place set all members to 0
            void zero() {
                for (int i = 0; i < size; i++) {
                    vector[i] = 0;
                }
            }

            // Update all values of vector based on index and value
            void update(std::function<T(int, T)> lambda) {
                for (int i = 0; i < size; i++) {
                    vector[i] = lambda(i, vector[i]);
                }
            }

            size_t getSize() const {
                return size;
            }

            std::string toString() const {
                std::string out = "[";
                for (int i = 0; i < size; i++) {
                    if (i > 0) out += ", ";
                    out += std::to_string(vector[i]);
                }
                out += "]";
                return out;
            }

            friend std::ostream& operator <<(std::ostream& out, const Vector& vect) {
                out << vect.toString();
                return out;
            }

            operator std::string() const {
                std::ostringstream out;
                out << *this;
                return out.str();
            }

            const T magnitude() {
                T sum = 0;
                for (int i = 0; i < size; i++) { sum += vector[i] * vector[i]; }
                return sqrt(sum);
            }

            const T magnitudeSq() {
                T mag = magnitude();
                return mag * mag;
            }

            Vector normalized() const {
                Vector copy = *this;
                T mag = copy.magnitude();
                if (mag != 0) {
                    for (int i = 0; i < size; i++) {
                        copy[i] /= mag;
                    }
                }
                return copy;
            }

            void normalize() {
                T mag = magnitude();
                if (mag != 0) {
                    for (int i = 0; i < size; i++) {
                        vector[i] /= mag;
                    }
                }

            }

            Vector rounded(float step = 1.0f) const {
                Vector copy = *this;
                for (int i = 0; i < size; i++) {
                    copy[i] = std::round(copy[i]);
                    //copy[i] = (float)std::floor(copy[i] * step + 0.5f) / step;
                }
                return copy;
            }

            void round() {
                for (int i = 0; i < size; i++) { vector[i] = std::round(vector[i]); }
            }

            Vector floor() const {
                Vector copy = *this;
                for (int i = 0; i < size; i++) { copy[i] = std::floor(copy[i]); }
                return copy;
            }

            Vector ceil() const {
                Vector copy = *this;
                for (int i = 0; i < size; i++) { copy[i] = std::ceil(copy[i]); }
                return copy;
            }

            T dot(Vector vect) const {
                T sum = 0;
                for (int i = 0; i < size; i++) {
                    sum += vector[i] * vect[i];
                }
                return sum;
            }

            T angleBetween(Vector vect) {
                return atan2(cross(vect).dot(Vector::Face()), dot(vect));
            }

            Quaternion<T> rotationBetween(Vector vect) {

                float dotProd = dot(vect);
                float k = sqrt(magnitudeSq() * vect.magnitudeSq());

                if (dotProd / k == -1) {
                    // TODO: fix this
                    return Quaternion<T>(0, orth().normalized());
                }

                Quaternion q = Quaternion(0.5f * PI * (dotProd + k), cross(vect));
                q.normalize();

                return q;
            }

            // Compute an orthaganol vector
            Vector orth() const {
                if (dot(Vector::Top()) == 0.0) {
                    return cross(Vector::Top());
                }
                else if (dot(Vector::Right()) == 0.0) {
                    return cross(Vector::Right());
                }
                else {
                    return cross(Vector::Face());
                }
            }

            // Return the vector such that V.prod(V.inverse()) = V.Identity();
            Vector inverse() const {
                Vector copy = *this;
                for (int i = 0; i < size; i++) {
                    copy[i] = 1 / vector[i];
                };
                return copy;
            }


            Vector<3, T> cross(Vector vect) const {
                return Vector<3, T>(
                    vector[1] * vect[2] - vector[2] * vect[1],
                    -(vector[0] * vect[2] - vector[2] * vect[0]),
                    vector[0] * vect[1] - vector[1] * vect[0]
                );
            }

            // element-wise multiplication
            Vector prod(Vector vect) const {
                Vector copy = *this;
                for (int i = 0; i < size; i++) {
                    copy[i] *= vect[i];
                };
                return copy;
            }

            // element-wise division
            Vector div(Vector vect) const {
                Vector copy = *this;
                for (int i = 0; i < size; i++) {
                    copy[i] /= vect[i];
                };
                return copy;
            }

            // Return vect projected onto this vector
            Vector proj(Vector vect) const {
                return vect * (dot(vect) / vect.dot(vect));
            }

            // Cast vector from type T to V
            template <class V>
            Vector<size, V> cast() {
                Vector<size, V> copy = Vector<size, V>();
                for (int i = 0; i < size; i++) {
                    copy[i] = static_cast<V>(vector[i]);
                }
                return copy;
            }

            template <size_t toSize>
            Vector<toSize, T> resize(T defaultValue = 0.0) const {
                Vector<toSize, T> copy = Vector<toSize, T>();
                for (int i = 0; i < toSize; i++) {
                    if (i < size) {
                        copy.vector[i] = vector[i];
                    }
                    else {
                        copy.vector[i] = defaultValue;
                    }
                }
                return copy;
            }

            T min() const {
                return *std::min_element(vector.begin(), vector.end());
            }

            T max() const {
                return *std::max_element(vector.begin(), vector.end());
            }

            Vector bounded(Vector by) const {
                auto out = copy();
                float maxDim = max();
                float byMaxDim = by.max();
                float scale = maxDim / byMaxDim;

                if (scale > 1) {
                    out *= 1 / scale;
                }

                return out;
            }

            Vector fill(Vector to) const {
                auto out = copy();
                float scale = max() / to.max();
                return out * (1 / scale);
            }

            void clamp(T low, T high) {
                for (int i = 0; i < size; i++) {
                    vector[i] = std::clamp(vector[i], low, high);
                }
            }

            void clamp(Vector low, Vector high) {
                for (int i = 0; i < size; i++) {
                    vector[i] = std::clamp(vector[i], low[i], high[i]);
                }
            }

            // Operator Overloads

            T operator[](const int i) const {
                return vector[i];
            }

            T& operator[](const int i) {
                return vector[i];
            }

            bool operator==(const Vector& vect) const {
                for (int i = 0; i < size; i++) {
                    if (vector[i] != vect[i]) return false;
                }
                return true;
            }

            bool operator!=(const Vector& vect) const {
                for (int i = 0; i < size; i++) {
                    if (vector[i] != vect[i]) return true;
                }
                return false;
            }

            bool operator<(const Vector& vect) const noexcept {
                T magA, magB = 0.0f;
                for (int i = 0; i < size; i++) {
                    magA += vector[i] * vector[i];
                    magB += vect[i] * vect[i];
                }
                magA = sqrt(magA);
                magB = sqrt(magB);

                return magA < magB;
            }

            Vector operator+(const Vector& vect) const {
                Vector copy = *this;
                for (int i = 0; i < size; i++) {
                    copy[i] += vect[i];
                }
                return copy;
            }

            Vector operator-(const Vector& vect) const {
                Vector copy = *this;
                for (int i = 0; i < size; i++) {
                    copy[i] -= vect[i];
                }
                return copy;
            }

            Vector operator*(T scalar) const {
                Vector copy = *this;
                for (int i = 0; i < size; i++) {
                    copy[i] *= scalar;
                }
                return copy;
            }

            Vector operator/(T scalar) const {
                Vector copy = *this;
                for (int i = 0; i < size; i++) {
                    copy[i] /= scalar;
                }
                return copy;
            }

            Vector& operator+=(const Vector& vect) {
                for (int i = 0; i < size; i++) {
                    vector[i] += vect[i];
                }
                return *this;
            }

            Vector& operator-=(const Vector& vect) {
                for (int i = 0; i < size; i++) {
                    vector[i] -= vect[i];
                }
                return *this;
            }

            Vector& operator*=(T scalar) {
                for (int i = 0; i < size; i++) {
                    vector[i] *= scalar;
                }
                return *this;
            }

            Vector& operator/=(T scalar) {
                for (int i = 0; i < size; i++) {
                    vector[i] /= scalar;
                }
                return *this;
            }
        };

        template<size_t size, class T>
        Vector<size, T> operator *(T scalar, const Vector<size, T>& rhs) {
            Vector<size, T> copy = Vector<size, T>(rhs);
            return copy * scalar;
        }

        template <size_t size, class T>
        Vector<size, T> normalize(Vector<size, T> vector) {
            return vector.normalized();
        }

        template <size_t size, class T>
        Vector<size, T> cross(Vector<size, T> a, Vector<size, T> b) {
            return a.cross(b);
        }

        template <size_t size, class T>
        T dot(Vector<size, T> a, Vector<size, T> b) {
            return a.dot(b);
        }

        template <size_t size, class T>
        T dot2(Vector<size, T> a) {
            return a.dot(a);
        }

}
#endif //HAGAME2_VECTOR_H
