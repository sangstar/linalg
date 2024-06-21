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
    Matrix(std::vector<std::vector<T>> data) : data_(data)
    {
        num_rows_ = data.size();
        num_cols_ = data[0].size();
    }

    std::vector<std::vector<T>> data()
    {
        return data_;
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

    std::vector<T> get_column_vector(size_t col_index)
    {
        std::vector<T> col_vec;
        for (size_t i = 0; i < num_rows(); ++i)
        {
            col_vec.push_back(data_[col_index][i]);
        }
        return col_vec;
    }

    std::vector<T> get_row_vector(size_t row_index)
    {
        return data_[row_index];
    }

    std::string to_string()
    {
        std::stringstream ss;
        size_t skip_len = 4;
        auto add_whitespace = [skip_len, &ss]() -> void
        {
            for (size_t k = 0; k < skip_len; ++k)
            {
                ss << " ";
            }
        };

        ss << "Matrix([" << std::endl;
        for (size_t i = 0; i < num_rows(); ++i)
        {
            add_whitespace();
            ss << "[";
            for (size_t j = 0; j < num_cols(); ++j)
            {
                if (j == num_cols() - 1)
                {
                    ss << data_[i][j];
                }
                else
                {
                    ss << data_[i][j] << ", ";
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
    std::vector<std::vector<T>> data_;
    int num_rows_, num_cols_;
};

template <typename T>
float dot_product(std::vector<T> &v, std::vector<T> &u)
{
    float result = 0;
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

    std::vector<std::vector<T>> c_vectors;

    for (size_t i = 0; i < a.num_rows(); ++i)
    {
        std::vector<T> c_row_vector;
        std::vector<T> a_row_vector = a.get_row_vector(i);

        for (size_t j = 0; j < b.num_cols(); ++j)
        {
            std::vector<T> b_column_vector = b.get_column_vector(j);
            c_row_vector.push_back(dot_product(a_row_vector, b_column_vector));
        }

        c_vectors.push_back(c_row_vector);
    }

    return Matrix<T>(c_vectors);
}

#endif