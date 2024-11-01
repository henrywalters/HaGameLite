//
// Created by henry on 12/16/22.
//

#ifndef HAGAME2_MATRIX_H
#define HAGAME2_MATRIX_H

#include "vector.h"
#include "quaternion.h"
#include "hypercube.h"

namespace hg::math {
    enum MatrixSolutionType {
        Single,
        None,
        Infinite,
    };

    template <class T> inline T sq(T val) { return val * val; }

    template <size_t M, size_t N, class T>
    class Matrix {
    public:
        T mat[M * N];

        Matrix() {
            fill(0);
        }

        Matrix(T value) {
            fill(value);
        }

        Matrix(std::vector<T> values) {

            for (int i = 0; i < M * N; i++) {
                mat[i] = values[i];
            }
        }

        // Static Initializers

        static Matrix Identity() {
            Matrix mat;
            mat.fillDiag(1);
            return mat;
        }

        static Matrix<4, 4, T> Scale(Vector<3, T> scale) {
            std::vector<T> mat = {
                    scale.x(), 0, 0, 0,
                    0, scale.y(), 0, 0,
                    0, 0, scale.z(), 0,
                    0, 0, 0, 1,
            };
            return Matrix<4, 4, T>(mat);
        }

        static Matrix<4, 4, T> Translation(Vector<3, T> translation) {
            std::vector<T> trans = {
                    1, 0, 0, translation.x(),
                    0, 1, 0, translation.y(),
                    0, 0, 1, translation.z(),
                    0, 0, 0, 1
            };
            return Matrix<4, 4, T>(trans);
        }

        static Matrix<4, 4, T> Rotation(Quaternion<T> quat) {
            std::vector<T> rot = {
                    sq(quat[0]) + sq(quat[1]) - sq(quat[2]) - sq(quat[3]), 2 * quat[1] * quat[2] - 2 * quat[0] * quat[3], 2 * quat[1] * quat[3] + 2 * quat[0] * quat[2], 0,
                    2 * quat[1] * quat[2] + 2 * quat[0] * quat[3], sq(quat[0]) - sq(quat[1]) + sq(quat[2]) - sq(quat[3]), 2 * quat[2] * quat[3] - 2 * quat[0] * quat[1], 0,
                    2 * quat[1] * quat[3] - 2 * quat[0] * quat[2], 2 * quat[2] * quat[3] + 2 * quat[0] * quat[1], sq(quat[0]) - sq(quat[1]) - sq(quat[2]) + sq(quat[3]), 0,
                    0, 0, 0, 1
            };
            return Matrix<4, 4, T>(rot);
        }

        static Matrix<4, 4, T> Orthographic(T left, T right, T bottom, T top, T zNear, T zFar) {
            std::vector<T> orth = {
                    2 / (right - left), 0, 0, -(right + left) / (right - left),
                    0, 2 / (top - bottom), 0, -(top + bottom) / (top - bottom),
                    0, 0, -2 / (zFar - zNear), -(zFar + zNear) / (zFar - zNear),
                    0, 0, 0, 1
            };
            return Matrix<4, 4, T>(orth);
        }

        static Matrix<4, 4, T> Orthographic(Hypercube<3, T> projSpace) {
            return Matrix<4, 4, T>::Orthographic(
                    projSpace.pos[0],
                    projSpace.pos[0] + projSpace.size[0],
                    projSpace.pos[1],
                    projSpace.pos[1] + projSpace.size[1],
                    projSpace.pos[2],
                    projSpace.pos[2] + projSpace.size[2]
            );
        }

        static Matrix<4, 4, T> Perspective(Hypercube<3, T> projSpace) {
            return Matrix<4, 4, T>::Perspective(
                    projSpace.pos[0],
                    projSpace.pos[0] + projSpace.size[0],
                    projSpace.pos[1],
                    projSpace.pos[1] + projSpace.size[1],
                    projSpace.pos[2],
                    projSpace.pos[2] + projSpace.size[2]
            );
        }

        static Matrix<4, 4, T> Perspective(T fov, T aspect, T zNear, T zFar) {
            Matrix<4, 4, T> out;
            auto tanFov = std::tan(fov / 2);
            out.set(0, 0, 1.0f / (aspect * tanFov));
            out.set(1, 1, 1.0f / tanFov);
            out.set(2, 2, -(zFar + zNear) / (zFar - zNear));
            out.set(3, 2, -1.0f);
            out.set(2, 3, -(2.0f * zFar * zNear) / (zFar - zNear));
            return out;
        }

        static Matrix<4, 4, T> Perspective(T left, T right, T bottom, T top, T zNear, T zFar) {

            std::vector<T> proj = {
                    2 * zNear / (right - left), 0, (right + left) / (right - left), 0,
                    0, 2 * zNear / (top - bottom), (top + bottom) / (top - bottom), 0,
                    0, 0, -(zFar + zNear) / (zFar - zNear), -2 * zNear * zFar / (zFar - zNear),
                    0, 0, -1, 0
            };

            return Matrix<4, 4, T>(proj);
        }

        static Matrix<4, 4, T> LookAt(Vector<3, T> eye, Vector<3, T> target, Vector<3, T> up) {
            Vector<3, T> zAxis = (eye - target).normalized();
            Vector<3, T> xAxis = (up.cross(zAxis)).normalized();
            Vector<3, T> yAxis = zAxis.cross(xAxis);

            std::vector<T> view = {
                    xAxis[0], xAxis[1], xAxis[2], -xAxis.dot(eye),
                    yAxis[0], yAxis[1], yAxis[2], -yAxis.dot(eye),
                    zAxis[0], zAxis[1], zAxis[2], -zAxis.dot(eye),
                    0, 0, 0, 1
            };

//            std::vector<T> view = {
//                    xAxis[0], yAxis[0], zAxis[0], 0,
//                    xAxis[1], yAxis[1], zAxis[1], 0,
//                    xAxis[2], yAxis[2], zAxis[2], 0,
//                    -xAxis.dot(eye), -yAxis.dot(eye), -zAxis.dot(eye), 1
//            };

            return Matrix<4, 4, T>(view);
        }

        static Matrix<4, 4, T> FPSView(Vector<3, T> eye, float pitch, float yaw) {
            float cosPitch = cos(pitch);
            float sinPitch = sin(pitch);
            float cosYaw = cos(yaw);
            float sinYaw = sin(yaw);

            Vector<3, T> xAxis = Vector<3, T>({ cosYaw, 0, -sinYaw });
            Vector<3, T> yAxis = Vector<3, T>({ sinYaw * sinPitch, cosPitch, cosYaw * sinPitch });
            Vector<3, T> zAxis = Vector<3, T>({ sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw });

            std::vector<T> view = {
                    xAxis[0], xAxis[1], xAxis[2], -xAxis.dot(eye),
                    yAxis[0], yAxis[1], yAxis[2], -yAxis.dot(eye),
                    zAxis[0], zAxis[1], zAxis[2], -zAxis.dot(eye),
                    0, 0, 0, 1
            };

            return Matrix<4, 4, T>(view);

        }

        // Class methods

        template<size_t P, size_t Q>
        Matrix<P, Q, T> resize() {
            Matrix<P, Q, T> copy = Matrix<P, Q, T>();
            for (int i = 0; i < P; i++) {
                for (int j = 0; j < Q; j++) {
                    if (i <= M && j <= N) {
                        copy.set(i, j, get(i, j));
                    }
                    else {
                        copy.set(i, j, 0);
                    }
                }
            }
            return copy;
        }

        // Create a shallow copy of a matrix
        Matrix<M, N, T> copy() {
            Matrix<M, N, T> c = Matrix<M, N, T>();
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++) {
                    c.set(i, j, get(i, j));
                }
            }
            return c;
        }

        // Shallowly assign all the values of a matrix from another
        void assign(Matrix<M, N, T> mat) {
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++) {
                    set(i, j, mat.get(i, j));
                }
            }
        }

        const static int rows() {
            return N;
        }

        const static int cols() {
            return M;
        }

        const T get(int row, int col) {
            return mat[Matrix::FlattenIndex(row, col)];
        }

        const void set(int row, int col, T value) {
            mat[Matrix::FlattenIndex(row, col)] = value;
        }

        const void forEach(std::function<void(int, int)> lambda) {
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++) {
                    lambda(j, i);
                }
            }
        }

        Vector<N, T> getCol(int col) {
            Vector<N, T> out;
            for (int i = 0; i < N; i++) {
                out[i] = get(i, col);
            }
            return out;
        }

        Vector<M, T> getRow(int row) {
            Vector<M, T> out;
            for (int i = 0; i < M; i++) {
                out[i] = get(row, i);
            }
            return out;
        }

        void setCol(int col, Vector<N, T> values) {
            for (int i = 0; i < N; i++) {
                set(i, col, values[i]);
            }
        }

        void setRow(int row, Vector<M, T> values) {
            for (int i = 0; i < M; i++) {
                set(row, i, values[i]);
            }
        }

        bool isSquare() {
            return M == N;
        }

        void fill(T value) {
            for (int i = 0; i < M * N; i++) {
                mat[i] = value;
            }
        }

        void swapRows(int rowA, int rowB) {
            float tmp;
            for (int i = 0; i < N; i++) {
                tmp = get(rowA, i);
                set(rowA, i, get(rowB, i));
                set(rowB, i, tmp);
            }
        }

        void addRowMultiple(int rowA, int rowB, T scalar) {
            for (int i = 0; i < N; i++) {
                set(rowA, i, get(rowA, i) + get(rowB, i) * scalar);
            }
        }

        void multiplyRow(int row, T scalar) {
            for (int i = 0; i < N; i++) {
                set(row, i, get(row, i) * scalar);
            }
        }

        void fillDiag(T value) {
            if (!isSquare()) {
                throw std::invalid_argument("Can only fill diag of square matrices");
            }

            for (int i = 0; i < M; i++) {
                mat[Matrix::FlattenIndex(i, i)] = value;
            }
        }

        Matrix<M, N, T> inverted() {
            Matrix<M, N, T> orig = copy();
            Matrix<M, N, T> eye = Matrix<M, N, T>::Identity();

            for (int i = 0; i < M; i++) {

                T scalar = orig.get(i, i);
                orig.multiplyRow(i, 1 / scalar);
                eye.multiplyRow(i, 1 / scalar);

                for (int j = 0; j < N; j++) {
                    if (i != j && orig.get(j, i) != 0) {
                        T temp = orig.get(j, i);
                        orig.addRowMultiple(j, i, -temp);
                        eye.addRowMultiple(j, i, -temp);
                    }
                }
            }

            return eye;
        }

        void invert() {
            assign(inverted());
        }

        Matrix<M, N, T> transpose() {
            Matrix<M, N, T> out = copy();
            if (M == N) {
                for (int i = 0; i < M; i++) {
                    for (int j = 0; j < N; j++) {
                        out.set(i, j, get(j, i));
                    }
                }
            }
            else {
                throw std::bad_function_call();
            }

            return out;
        }

        std::string toString() const {
            std::string out = "";
            for (int i = 0; i < M; i++) {
                std::string row = "[";
                for (int j = 0; j < N; j++) {
                    if (j > 0) row += ", ";
                    row += std::to_string(mat[Matrix::FlattenIndex(i, j)]);
                }
                row += "]\n";
                out += row;
            }

            return out;
        }

        friend std::ostream& operator <<(std::ostream& out, const Matrix& mat) {
            out << mat.toString();
            return out;
        }

        operator std::string() const {
            std::ostringstream out;
            out << *this;
            return out.str();
        }

        // Static helpers
        static int FlattenIndex(int row, int col) {
            return row * N + col;
        }

        const static bool IsSameSize(const Matrix& A, const Matrix& B) {
            return A.cols() == B.cols() && A.rows() == B.rows();
        }

        const static bool CanMultiply(const Matrix& A, const Matrix& B) {
            return A.cols() == B.rows();
        }

        static Vector<2, int> UnpackIndex(int flatIndex) {
            Vector<2, int> idx = Vector<2, int>();
            idx[1] = flatIndex % N;
            idx[0] = (flatIndex - idx[1]) / N;
            return idx;
        }

        // Operators
        T operator[](int i) const {
            return mat[i];
        }

        T& operator[](int i) {
            return mat[i];
        }

        Matrix operator+(const Matrix& B) {
            if (!Matrix::IsSameSize(this, B)) {
                throw std::invalid_argument("Only same size matrices may be added");
            }

            Matrix out = Matrix();
            for (int i = 0; i < M * N; i++) {
                out[i] = this[i] + B[i];
            }
            return out;
        }

        Matrix operator+=(const Matrix& B) {
            if (!Matrix::IsSameSize(this, B)) {
                throw std::invalid_argument("Only same size matrices may be added");
            }
            for (int i = 0; i < M * N; i++) {
                this[i] += B[i];
            }
        }

        template <size_t P, size_t Q>
        Matrix operator*(const Matrix<P, Q, T>& B) {
            if (!Matrix::CanMultiply(*this, B)) {
                throw std::invalid_argument("Number of cols in mat A must match the number of rows in mat B");
            }

            Matrix<M, Q, T> out = Matrix<M, Q, T>();
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < B.cols(); j++) {
                    for (int k = 0; k < B.rows(); k++) {
                        out.set(i, j, out.get(i, j) + (get(i, k) * B[Matrix::FlattenIndex(k, j)]));
                    }
                }
            }
            return out;
        }

        template <size_t P>
        Vector<P, T> operator*(const Vector<P, T>& B) {
            Vector<P, T> out = Vector<P, T>();
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++) {
                    out[i] += B[j] * get(i, j);
                }
            }
            return out;
        }
    };

    template <size_t N, size_t M, size_t, size_t P, size_t Q, class T>
    Matrix<M, Q, T> operator * (const Matrix<N, M, T>& A, const Matrix<P, Q, T>& B) {
        Matrix<M, Q, T> out = Matrix<M, Q, T>();
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < B.cols(); j++) {
                for (int k = 0; k < B.rows(); k++) {
                    out[i * N + j] += A[i * N + k] * B[k * P + j];
                }
            }
        }
        return out;
    }

    template <size_t N, size_t P, size_t Q, class T>
    Vector<N, T> operator * (const Vector<N, T>& vec, const Matrix<P, Q, T>& mat) {
        Vector<Q, T> out = Vector<Q, T>();
        for (int i = 0; i < Q; i++) {
            for (int j = 0; j < N; j++) {
                out[i] += vec[j] * mat[j * P + i];
            }
        }
        return out;
    }
}


#endif //HAGAME2_MATRIX_H
