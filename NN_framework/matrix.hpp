#pragma once
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>
#include <tuple>
#include <utility>
#include <functional>
#include <concepts>

template <typename T> 
concept Arithmetic = std::integral<T> || std::floating_point<T>;
template <Arithmetic T = float>
class Matrix {
    unsigned int m_width;
    unsigned int m_height;
    std::vector<T> m_vec;

public:
    Matrix() :
        m_width(0), m_height(0), m_vec(m_height* m_width, 0)
    {
        //do nothing
    }

    Matrix(const unsigned int width, const unsigned int height) :
        m_width(width), m_height(height), m_vec(m_height* m_width, 0)
    {
        //do nothing
    }

    Matrix(const unsigned int width, const unsigned int height, std::vector<T> vec) :
        m_width(width), m_height(height), m_vec(vec)
    {
        if (m_vec.size() != m_width * m_height) {
            throw std::logic_error("Wrong vec size");
        }
    }

    unsigned int width() const {
        return m_width;
    }

    unsigned int height() const {
        return m_height;
    }

    std::vector<T> const& vec() const {
        return m_vec;
    }

    std::vector<T>& vec() {
        return m_vec;
    }

    Matrix<T> operator+(Matrix<T> const& target) {
        return add(target);
    }

    Matrix<T> operator-() const {
        std::vector<T> opposite_vec(m_height * m_width);
        for (int i = 0; i < m_height*m_width; ++i) {
            opposite_vec[i] = -m_vec[i];
        }
        Matrix opposite_mat(m_width, m_height, opposite_vec);
        return opposite_mat;
    }

    Matrix<T> operator-(Matrix<T> const& target) {
        return sub(target);
    }

    T getValue(const int x, const int y) const {

        if (x >= 0 && y >= 0 && x < m_width && y < m_height) {
            return m_vec[y * m_width + x];
        }
        else {
            throw std::logic_error("Looking for value outside the matrix");
        }
    }

    void setValue(const int x, const int y, const T new_val) {
        if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
            m_vec[y * m_width + x] = new_val;
        }
    }

    void display() const {
        for (int i = 0; i < m_height; ++i) {
            for (int j = 0; j < m_width; ++j) {
                std::cout << getValue(j, i) << "|";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }


    Matrix<T> add(Matrix<T> const& mat) {

        if (m_width == mat.width() && m_height == mat.height()) {
            std::vector<T> sum_vec = m_vec;

            for (int i = 0; i < m_width * m_height; ++i) {
                sum_vec[i] += mat.vec()[i];
            }

            Matrix<T> sum_mat(m_width, m_height, sum_vec);
            return sum_mat;
        }

        else {
            throw std::logic_error("Adding two matrix with different dimensions");
        }
    }

    Matrix<T> sub(Matrix const& mat) {
        Matrix<T> neg_mat = -mat;
        return add(neg_mat);
    }

    Matrix<T> transpose() const {
        Matrix<T> t_mat(m_height, m_width, m_vec);
        for (int y = 0; y < m_height; ++y) {
            for (int x = 0; x < m_width; ++x) {
                t_mat.setValue(y, x, getValue(x, y));
            }
        }
        return t_mat;
    }

    Matrix<T> multiply(const T scalar) {
        std::vector<T> mul_vec(m_width*m_height);

        for (int i = 0; i < m_width * m_height; ++i) {
            mul_vec[i] = m_vec[i] * scalar;
        }

        Matrix<T> mul_mat(m_width, m_height, mul_vec);
        return mul_mat;
    }

    Matrix<T> multiply(Matrix const& mat) {
        Matrix<T> mul_mat(mat.width(), m_height);

        if (m_width == mat.height()) {
            for (int i = 0; i < mul_mat.width(); ++i) {
                for (int j = 0; j < mul_mat.height(); ++j) {
                    T mul_res = 0;
                    for (int k = 0; k < m_width; ++k) {
                        mul_res += getValue(k,j) * mat.getValue(i,k);
                    }
                    mul_mat.setValue(i, j, mul_res);
                }
            }
        }
        else {
            throw std::logic_error("Multiplication failed");
        }

        return mul_mat;
    }

    Matrix<T> multiply_elementwise(Matrix const& mat) {
        Matrix<T> mul_mat(m_width, m_height);

        if (m_width == mat.width() && m_height == mat.height()) {
            for (int i = 0; i < m_width; ++i) {
                for (int j = 0; j < m_height; ++j) {
                    mul_mat.setValue(i, j, getValue(i, j) * mat.getValue(i, j));
                }
            }
        }
        else {
            throw std::logic_error("Multiplication elementwise failed");
        }
        return mul_mat;
    }

    Matrix<T> applyFunction(std::function<float (const float&)> func) {
        Matrix<T> output(m_width, m_height);

        for (int i = 0; i < m_width; ++i) {
            for (int j = 0; j < m_height; ++j) {
                output.setValue(i, j, func(getValue(i, j)));
            }
        }

        return output;
    }

    Matrix<T> slice(const int start, const int end, const int axis) {
        int xs = 0;
        int xe = m_width;
        int ys = 0;
        int ye = m_height;
        
        if (axis == 0) {
            xs = start;
            xe = end;
        }
        else if (axis == 1) {
            ys = start;
            ye = end;
        }
        else {
            throw std::logic_error("Impossible to slice on other axis than 0 and 1");
        }

        Matrix<T> output(xe - xs, ye - ys);
        for (int i = xs; i < xe; ++i) {
            for (int j = ys; j < ye; ++j) {
                output.setValue(i - xs, j - ys, getValue(i, j));
            }
        }
        return output;
    }
};


Matrix<float> randomMatrix(const unsigned int width, const unsigned int height) {
    std::vector<float> vec(width * height);
    for (int i = 0; i < width * height; ++i) {
        vec[i] = (float)rand() / RAND_MAX;
    }
    Matrix<float> out(width, height, vec);
    return out;
}

Matrix<float> oneMatrix(const unsigned int width, const unsigned int height) {
    std::vector<float> vec(width * height, 1);
    Matrix<float> out(width, height, vec);
    return out;
}

Matrix<float> zeroMatrix(const unsigned int width, const unsigned int height) {
    std::vector<float> vec(width * height, 0);
    Matrix<float> out(width, height, vec);
    return out;
}

Matrix<float> identityMatrix(const unsigned int width, const unsigned int height) {
    std::vector<float> vec(width * height, 0);
    unsigned int n = height;
    if (width < height) {
        n = width;
    }
    for (int i = 0; i < n; ++i) {
        vec[i*width+i] = 1;
    }
    Matrix<float> out(width, height, vec);
    return out;
}