#pragma once
#include <vector>
#include <numeric>
#include <algorithm>

template <typename T>
void DisplayVec(std::vector<T> vec) {
    for (int i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << "|";
    }
    std::cout << std::endl;
}

template <typename T>
std::vector<T> slicing(std::vector<T>& arr, int X, int Y){

    // Starting and Ending iterators
    auto start = arr.begin() + X;
    auto end = arr.begin() + Y;

    // To store the sliced vector
    std::vector<T> result(Y - X);

    // Copy vector using copy function()
    copy(start, end, result.begin());

    // Return the final sliced vector
    return result;
}

float average(std::vector<float> const& v) {
    if (v.empty()) {
        return 0;
    }

    auto const count = static_cast<float>(v.size());
    return std::reduce(v.begin(), v.end()) / count;
}

bool check_matrices_shapes(std::vector<Matrix<float>>& matrix_list) {
    int n_sample = matrix_list[0].width();
    bool shape_check = true;
    for (int i = 1; i < matrix_list.size(); ++i) {
        if (matrix_list[i].width() != n_sample) {
            shape_check = false;
            break;
        }
    }
    return shape_check;
}

template <typename T>
void shuffle(std::vector<Matrix<T>>& matrix_list) {

    int n_sample = matrix_list[0].width();
    bool shape_check = check_matrices_shapes(matrix_list);

    if (shape_check) {
        for (int i = 0; i < n_sample; ++i) {
            int idx1 = rand() % n_sample;
            int idx2 = rand() % n_sample;
            for (int j = 0; j < matrix_list.size(); ++j) {
                for (int k = 0; k < matrix_list[j].height(); ++k) {
                    float temp = matrix_list[j].getValue(idx1, k);
                    matrix_list[j].setValue(idx1, k, matrix_list[j].getValue(idx2, k));
                    matrix_list[j].setValue(idx2, k, temp);
                }
            }
        }
    }
    else {
        throw std::logic_error("Matrices are not the same width");
    }
}
template <typename T>
std::vector<Matrix<T>> split_train_test(std::vector<Matrix<T>>& matrix_list, float train_ratio) {
    
    bool shape_check = check_matrices_shapes(matrix_list);
    if (train_ratio < 0 || train_ratio>1) {
        throw std::logic_error("train_ratio should be between 0 and 1");
    }
    else if (!shape_check) {
        throw std::logic_error("Matrices are not the same width");
    }
    else {
        int n_sample = matrix_list[0].width();
        int train_size = (int)n_sample * train_ratio;

        shuffle(matrix_list);
        std::vector<Matrix<T>> res;
        
        for (int i = 0; i < matrix_list.size(); ++i) {
            Matrix train_mat = matrix_list[i].slice(0, train_size, 0);
            res.push_back(train_mat);
            Matrix test_mat = matrix_list[i].slice(train_size, matrix_list[i].width(), 0);
            res.push_back(test_mat);
        }
        return res;
    }
}

template <typename T>
Matrix<T> normalizeMatrix(Matrix<T>& mat) {
    std::vector<T> vec = mat.vec();
    const auto [vmin, vmax] = std::minmax_element(begin(vec), end(vec));
    std::cout << *vmin << ";" << *vmax << std::endl;
    Matrix<T> A = oneMatrix(mat.width(), mat.height());
    A = A.multiply(1 / (*vmax - *vmin));
    Matrix<T> B = A.multiply(- *vmin);
    Matrix<T> res = A.multiply_elementwise(mat) + B;
    return res;
}
