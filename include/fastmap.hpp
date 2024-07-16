#ifndef LINALG_FASTMAP_HPP
#define LINALG_FASTMAP_HPP

#include <cmath>
#include <iostream>
#include <vector>
#include <ranges>
#include <omp.h>
#include "linalg.hpp"

namespace views = std::views;

template<typename T>
inline T euclidean_distance(const std::vector<T> &a, const std::vector<T> &b) {
    T distance = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        distance += pow(a[i] - b[i], 2);
    }
    return sqrt(distance);
}

template<typename T>
int argmax(const std::vector<T> &a) {
    T max = a[0];
    int max_idx = 0;
    for (size_t i = 1; i < a.size(); ++i) {
        if (a[i] > max) {
            max = a[i];
            max_idx = i;
        }
    }
    return max_idx;
}

template<typename T>
int get_pivot_point(const Matrix<T> &data, int pivot_point, int num_points, Matrix<T>& distances) {
    std::vector<T> pivot_distances(num_points);
    #pragma omp parallel for
    for (int i = 0; i < num_points; ++i) {
        pivot_distances[i] = distances.at(pivot_point, i);
    }
    return argmax(pivot_distances);
}

template<typename T>
Matrix<T> get_distance_matrix(Matrix<T> &X, int n) {
    // Initialize the distances vector with the correct size
    std::vector<std::vector<T>> distances(n, std::vector<T>(n));

    // Use symmetry to reduce the number of computations
    #pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            if (i == j) {
                distances[i][j] = 0;
            } else {
                T dist = euclidean_distance(X.get_row(i), X.get_row(j));
                distances[i][j] = dist;
                distances[j][i] = dist;  // Symmetric assignment
            }
        }
    }

    return Matrix(distances);
}

template<typename T>
Matrix<T> reduce_with_fastmap(Matrix<T> &X, int target_dim) {
    int n = X.num_rows();
    auto reduced = Matrix<T>(n, target_dim);

    // Figure out a more efficient way to do this. This is prohibitively large
    // Try using a non-C-contiguous matrix in this case. A vector of vectors
    Matrix<T> distances = get_distance_matrix(X, n);

    for (int j: views::iota(0, target_dim)) {
        int O_a = 0;
        int O_b = get_pivot_point(X, O_a, n, distances);
        O_a = get_pivot_point(X, O_b, n, distances);

        T D_pivots = distances.at(O_a,O_b);
        if (D_pivots == 0) {
            break;
        }

        #pragma omp parallel for
        for (int i = 0; i < n; ++i) {
            T d_ai = distances.at(O_a, i);
            T d_bi = distances.at(O_b, i);
            reduced.at(i, j) = (d_ai * d_ai + D_pivots * D_pivots - d_bi * d_bi) / (2 * D_pivots);
        }

        #pragma omp parallel for
        for (int i = 0; i < n; ++i) {
            for (int l = i + 1; l < n; ++l) {
                T d_ij = distances.at(i, l);
                T d_proj = std::abs(d_ij * d_ij - pow(reduced.at(i, j) - reduced.at(l, j), 2));
                distances.at(i, l) = std::sqrt(d_proj);
                distances.at(l, i) = distances.at(i,l);  // Symmetric update
            }
        }
    }
    return reduced;
}

#endif // LINALG_FASTMAP_HPP
