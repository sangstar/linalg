#ifndef LINALG_H
#define LINALG_H

#include <iostream>
#include <vector>
#include <sstream>
#include <string>

template <typename T>
class Matrix
{
public:
    Matrix(std::vector<T> data, size_t num_rows, size_t num_cols)
        : data_(data), num_rows_(num_rows), num_cols_(num_cols){};

    // Additionally add a constructor for a vector of vectors
    // (so that the Python lib can initialize with a list of lists)
    explicit Matrix(std::vector<std::vector<T>> stacked_data)
    {
        num_rows_ = stacked_data.size();
        num_cols_ = stacked_data[0].size();
        data_.reserve(num_rows_ * num_cols_);

        for (size_t i = 0; i < num_rows_; ++i)
        {
            for (size_t j = 0; j < num_cols_; ++j)
            {
                data_.push_back(stacked_data[i][j]);
            }
        }
    }

    std::vector<T> &data()
    {
        return data_;
    }

    // TODO: Add []operator
    T &at(size_t row, size_t col)
    {
        return data_[row * num_cols_ + col];
    }

    size_t
    num_rows() const
    {
        return num_rows_;
    }

    size_t num_cols() const
    {
        return num_cols_;
    }

    // Get row-echelon form using LU decomposition, performed in-place
    void echelon(bool take_upper, bool take_lower) {
        if (num_cols_ != num_rows_) {
            throw std::runtime_error("Cannot reduce a rectangular matrix");
        }

        // This outer loop i runs through each pivot element
        // on the diagonal of Matrix
        for (size_t i = 0; i < num_rows_; ++i){

            for (size_t k = i; k < num_rows_; ++k) {
                T sum = 0;

                // Since L stores the pivot factors for each
                // row, it uses it to update the values in
                // the pivot row. For instance, to find U[1][1],
                // one needs to find the second diagonal element
                // of Matrix, the second pivot. This requires
                // modifying R1 by R1 = R1 - b*R0 where b zeros
                // out the element one row under the pivot. The
                // first L element of that row is that factor,
                // represented by at(i,j) here.
                for (size_t j = 0; j < i; ++j) {
                    sum += at(i, j) * at(j, k);
                }

                at(i, k) = at(i, k) - sum;
            }
            // Calculate the lower triangular matrix L
            // This essentially attempts to find the pivot factor
            // but instead of using it to zero out rows elements
            // below the pivot, it stores the pivot factor where it
            // would've zero'd
            for (size_t k = i + 1; k < num_rows_; ++k) {
                T sum = 0;

                for (size_t j = 0; j < i; ++j) {
                    sum += at(k, j) * at(j, i);
                }

                at(k, i) = (at(k, i) - sum) / at(i,i);
            }
        }

        // This result combines the L and U matrices respectively in their upper and lower
        // triangular quadrants.

        // Zero out the elements below the diagonal to retrieve U, which is the original
        // Matrix in echelon form
        if (take_upper) {
            for (size_t i = 0; i < num_rows_; ++i) {
                for (size_t j = 0; j < num_rows_; ++j) {
                    if (j < i) {
                        at(i, j) = 0;
                    }
                }
            }
        }
        if (take_lower) {
            for (size_t i = 0; i < num_rows_; ++i) {
                for (size_t j = 0; j < num_rows_; ++j) {
                    if (i == j) {
                        at(i,j) = 1;
                    }
                    if (j > i) {
                        at(i, j) = 0;
                    }
                }
            }
        }

    }

    // TODO: Ensure whitespace length of all rows
    //       is as long as the longest row
    std::string to_string()
    {
        std::stringstream ss;

        ss << "Matrix([" << std::endl;
        for (size_t i = 0; i < num_rows_; ++i)
        {
            ss << "    ";
            ss << "[";
            for (size_t j = 0; j < num_cols_; ++j)
            {
                if (j == num_cols() - 1)
                {
                    ss << at(i, j);
                }
                else
                {
                    ss << at(i, j) << ", ";
                }
            }
            if (i == num_rows() - 1)
            {
                ss << "]" << std::endl;
                ss << "])";
            }
            else
            {
                ss << "]," << std::endl;
            }
        }
        return ss.str();
    }

    ~Matrix()= default;

private:
    std::vector<T> data_;
    size_t num_rows_, num_cols_;
};

template <typename T>
float dot_product(std::vector<T> &v, std::vector<T> &u)
{
    T result = 0;
    if (v.size() != u.size())
    {
        throw std::runtime_error("Incompatible vector lengths.");
    }

    for (size_t i = 0; i < v.size(); ++i)
    {
        result += v[i] * u[i];
    }
    return result;
}

template <typename T>
Matrix<T> matmul(Matrix<T> &a, Matrix<T> &b)
{
    if (a.num_cols() != b.num_rows())
    {
        throw std::runtime_error("Incompatible matrix dimensions.");
    }

    std::vector<T> c_data;

    // m indexes over the rows of a
    for (size_t m = 0; m < a.num_rows(); ++m)
    {
        // p indexes over the cols of b
        for (size_t p = 0; p < b.num_cols(); ++p)
        {
            // n indexes over the cols of a and the rows of b
            T dot_product_result = 0;
            for (size_t n = 0; n < b.num_rows(); ++n)
            {
                dot_product_result += a.at(m, n) * b.at(n, p);
            }
            c_data.push_back(dot_product_result);
        }
    }

    return Matrix<T>(c_data, a.num_rows(), b.num_cols());
}

#endif