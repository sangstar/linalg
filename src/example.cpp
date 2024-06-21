#include "linalg.hpp"
#include <vector>
#include <iostream>

int main()
{
    auto v = std::vector<int>{1, 2, 3};
    auto u = std::vector<int>{4, 5, 6};

    std::cout << dot_product(v, u);

    auto a1 = std::vector<int>{1, 2, 3};
    auto a2 = std::vector<int>{4, 5, 6};

    auto b1 = std::vector<int>{1, 2, 3};
    auto b2 = std::vector<int>{4, 5, 6};
    auto b3 = std::vector<int>{7, 8, 9};

    std::vector<std::vector<int>> a_vectors = {a1, a2};
    std::vector<std::vector<int>> b_vectors = {b1, b2, b3};

    auto mat_a = Matrix<int>(a_vectors);
    auto mat_b = Matrix<int>(b_vectors);

    Matrix<int> result = matmul(mat_a, mat_b);

    result.to_string();

    return 0;
}