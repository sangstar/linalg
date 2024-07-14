//
// Created by Sanger Steel on 7/13/24.
//

#ifndef LINALG_FASTMAP_HPP
#define LINALG_FASTMAP_HPP

#include <cmath>
#include <iostream>
#include <vector>
#include <ranges>
#include "linalg.hpp"

namespace views = std::views;


template <typename T>
std::vector<T> add_vectors(const std::vector<T>& a, const std::vector<T>& b) {
    if (a.size() != b.size()) {
        throw std::runtime_error("Incompatible sizes for addition");
    }

    std::vector<T> c;
    for (size_t i = 0; i < a.size(); ++i) {
        c.push_back(a.at(i) + b.at(i));
    }

    return c;
}

template <typename T>
std::vector<T> scale_vectors(const std::vector<T>& a, T by) {
    std::vector<T> c;
    for (size_t i = 0; i < a.size(); ++i) {
        c.push_back(a.at(i) * by);
    }

    return c;
}

template <typename T>
std::vector<T> subtract_vectors(const std::vector<T>& a, const std::vector<T>& b) {
    std::vector<T> minus_b = scale_vectors(b, T(-1));
    std::vector<T> c = add_vectors(a, minus_b);
    return c;
}

template <typename T>
T euclidean_distance(const std::vector<T>& a, const std::vector<T>& b){
    T distance = 0;
    std::vector<T> subtracted_vector = subtract_vectors(a, b);

    for (size_t i = 0; i < subtracted_vector.size(); ++i) {
        distance += pow(subtracted_vector.at(i), 2);
    }

    return sqrt(distance);

}

template <typename T>
int argmax(std::vector<T>& a){
    T max = 0;
    int max_idx = 0;
    for (int i = 0; i < a.size(); ++i) {
        if (a.at(i) >= max) {
            max = a.at(i);
            max_idx = i;
        }
    }
    return max_idx;
}


template <typename T>
int get_pivot_point(Matrix<T>& data, int pivot_point, int num_points) {
    std::vector<T> distances;
    for (int i: views::iota(0, num_points)) {
        distances.push_back(euclidean_distance(data.get_row(pivot_point), data.get_row(i)));
    }
    return argmax(distances);
}



template <typename T>
Matrix<T> reduce_with_fastmap(Matrix<T>& X, int target_dim) {
    int n = X.num_rows();
    auto reduced = Matrix<T>(n, target_dim);

    for (int j : views::iota(0, target_dim)) {
        int O_a = 0;
        int O_b = get_pivot_point(X, O_a, n);
        O_a = get_pivot_point(X, O_b, n);

        // Get distance between pivot points pair
        T D_pivots = euclidean_distance(X.get_row(O_a), X.get_row(O_b));
        if (D_pivots == 0) {
            break;
        }

        for (int i = 0; i < n; ++i) {
            reduced.at(i,j) = (
                    (std::pow(euclidean_distance(X.get_row(i), X.get_row(O_a)), 2) + std::pow(D_pivots, 2) - std::pow(euclidean_distance(X.get_row(i), X.get_row(O_b)), 2)) /
                    (2 * D_pivots)
            );
        }

        for (int i = 0; i < n; ++i) {
            for (int l = i + 1; l < n; ++l) {
                X.at(i,l) = std::sqrt(std::abs(std::pow(euclidean_distance(X.get_row(i), X.get_row(l)), 2) - std::pow((reduced.at(i,j) - reduced.at(l,j)), 2)));
            }
        }
    }
    return reduced;
}
#endif //LINALG_FASTMAP_HPP
