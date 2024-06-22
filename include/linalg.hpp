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
    Matrix(std::vector<std::vector<T>> stackedData)
    {
        num_rows_ = stackedData.size();
        num_cols_ = stackedData[0].size();
        data_.reserve(num_rows_ * num_cols_);

        for (size_t i = 0; i < num_rows_; ++i)
        {
            for (size_t j = 0; j < num_cols_; ++j)
            {
                data_.push_back(stackedData[i][j]);
            }
        }
    }

    std::vector<T> &data()
    {
        return data_;
    }

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

    std::string to_string()
    {
        std::stringstream ss;

        ss << "Matrix([" << std::endl;
        for (size_t i = 0; i < num_rows(); ++i)
        {
            ss << "    ";
            ss << "[";
            for (size_t j = 0; j < num_cols(); ++j)
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

    ~Matrix(){};

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