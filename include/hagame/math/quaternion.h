//
// Created by henry on 12/16/22.
//

#ifndef HAGAME2_QUATERNION_H
#define HAGAME2_QUATERNION_H

#include "vector.h"
#include "constants.h"

namespace hg::math {
    template <class T>
    class Quaternion : public Vector<4, T> {
    private:

    public:

        Quaternion() {
            Quaternion(0, Vector<3, T>(0, 0, 0));
        }

        Quaternion(T rotation, Vector<3, T> axisOfRotation) {
            this->vector[0] = cos(0.5 * rotation);
            this->vector[1] = axisOfRotation[0] * sin(0.5 * rotation);
            this->vector[2] = axisOfRotation[1] * sin(0.5 * rotation);
            this->vector[3] = axisOfRotation[2] * sin(0.5 * rotation);
        }

        Quaternion(T w, T i, T j, T k) {
            this->vector[0] = w;
            this->vector[1] = i;
            this->vector[2] = j;
            this->vector[3] = k;
        }

        Quaternion(Vector<3, T> euler) : Quaternion(euler[0], euler[1], euler[2]) {}

        Quaternion(T roll, T pitch, T yaw) {
            T cy = cos(yaw * 0.5);
            T sy = sin(yaw * 0.5);
            T cp = cos(pitch * 0.5);
            T sp = sin(pitch * 0.5);
            T cr = cos(roll * 0.5);
            T sr = sin(roll * 0.5);

            this->vector[0] = cr * cp * cy + sr * sp * sy;
            this->vector[1] = sr * cp * cy - cr * sp * sy;
            this->vector[2] = cr * sp * cy + sr * cp * sy;
            this->vector[3] = cr * cp * sy - sr * sp * cy;
        }

        Quaternion operator*(const Quaternion& quat) const {
            return Quaternion(
                    this->vector[0] * quat[0] - this->vector[1] * quat[1] - this->vector[2] * quat[2] - this->vector[3] * quat[3],
                    this->vector[0] * quat[1] + this->vector[1] * quat[0] + this->vector[2] * quat[3] - this->vector[3] * quat[2],
                    this->vector[0] * quat[2] - this->vector[1] * quat[3] + this->vector[2] * quat[0] + this->vector[3] * quat[1],
                    this->vector[0] * quat[3] + this->vector[1] * quat[2] - this->vector[2] * quat[1] + this->vector[3] * quat[0]
            );
        }

        Quaternion operator*(const Vector<3, T>& vec) const {
            return *this * Quaternion(0, vec[0], vec[1], vec[2]);
        }

        Quaternion operator*(T scalar) const {
            return Quaternion(
                    this->vector[0] * scalar,
                    this->vector[1] * scalar,
                    this->vector[2] * scalar,
                    this->vector[3] * scalar
            );
        }

        Quaternion operator/(T scalar) const {
            return Quaternion(
                    this->vector[0] / scalar,
                    this->vector[1] / scalar,
                    this->vector[2] / scalar,
                    this->vector[3] / scalar
            );
        }

        T magnitude() const {
            return sqrt(this->vector[0] * this->vector[0] + this->vector[1] * this->vector[1] + this->vector[2] * this->vector[2] + this->vector[3] * this->vector[3]);
        }

        Quaternion conjugate() const {
            return Quaternion(this->vector[0], -this->vector[1], -this->vector[2], -this->vector[3]);
        }

        Quaternion inverse() const {
            return conjugate() / magnitude();
        }

        Vector<3, T> imaginary() const {
            return Vector<3, T>(this->vector[1], this->vector[2], this->vector[3]);
        }

        T real() const {
            return this->vector[0];
        }

        Vector<3, T> rotatePoint(Vector<3, T> point) const {
            return ((*this * point) * conjugate()).imaginary();
        }

        Quaternion<T> normalized() const {
            T mag = magnitude();
            return (*this) / mag;
        }

        // Spherical Linear Interpolation (Slerp)
        Quaternion<T> slerp(const Quaternion<T>& target, T t) const {
            T dotProduct = dot(target);

            // If the dot product is negative, use the opposite direction
            if (dotProduct < 0.0f) {
                return slerp(-target, t);
            }

            // If the quaternions are very close, use linear interpolation
            if (dotProduct > 0.9995f) {
                return normalized() * (1 - t) + target.normalize() * t;
            }

            dotProduct = std::clamp(dotProduct, -1.0f, 1.0f);
            T theta_0 = acos(dotProduct); // angle between the two quaternions
            T theta = theta_0 * t; // angle at the interpolation point
            Quaternion<T> targetNorm = (target - (*this) * dotProduct).normalize(); // perpendicular quaternion

            return (*this) * cos(theta) + targetNorm * sin(theta);
        }

        // Computes the dot product between two quaternions
        T dot(const Quaternion<T>& quat) const {
            return this->vector[0] * quat[0] +
                   this->vector[1] * quat[1] +
                   this->vector[2] * quat[2] +
                   this->vector[3] * quat[3];
        }

        Vector<3, T> eulerAngles() const {
            T a = this->vector[0];
            T b = this->vector[1];
            T c = this->vector[2];
            T d = this->vector[3];

            // Roll
            T roll = atan2(2 * (a * b + c * d), 1 - 2 * (b * b + c * c));

            // Pitch
            T pitch = asin(2 * (a * c - d * b));

            // Clamp pitch to the range [-pi/2, pi/2]
            if (pitch > PI / 2) pitch = PI / 2;
            else if (pitch < -PI / 2) pitch = -PI / 2;

            // Yaw
            T yaw = atan2(2 * (a * d + b * c), 1 - 2 * (c * c + d * d));

            return Vector<3, T>(roll, pitch, yaw);
        }

    };

    template<class T>
    Quaternion<T> operator *(const Quaternion<T>& lhs, const Quaternion<T>& rhs) {
        return Quaternion(
                                  lhs[0] * rhs[0] - lhs[1] * rhs[1] - lhs[2] * rhs[2] - lhs[3] * rhs[3],
                                  lhs[0] * rhs[1] + lhs[1] * rhs[0] + lhs[2] * rhs[3] - lhs[3] * rhs[2],
                                  lhs[0] * rhs[2] - lhs[1] * rhs[3] + lhs[2] * rhs[0] + lhs[3] * rhs[1],
                                  lhs[0] * rhs[3] + lhs[1] * rhs[2] + lhs[2] * rhs[1] + lhs[3] * rhs[0]
                          );
    }

    template<class T>
    Quaternion<T> operator *(T scalar, const Quaternion<T>& rhs) {
        return Quaternion(
                                  rhs[0] * scalar,
                                  rhs[1] * scalar,
                                  rhs[2] * scalar,
                                  rhs[3] * scalar
                          );
    }

    template<class T>
    Quaternion<T> operator*(Vector<3, T> vec, const Quaternion<T>& rhs) {
        return Quaternion(0, vec[0], vec[1], vec[2]) * rhs;
    }
}

#endif //HAGAME2_QUATERNION_H
