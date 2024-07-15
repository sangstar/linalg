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

    Matrix(size_t num_rows, size_t num_cols) : num_rows_(num_rows), num_cols_(num_cols) {
        data_.reserve(num_rows_ * num_cols_);
        for (size_t i = 0; i < num_rows * num_cols; ++i) {
            data_.push_back(T(0));
        }
    }

    // Additionally add a constructor for a vector of vectors
    // (so that the Python lib can initialize with a list of lists)

    // Matrices are stored as simply a std::vector<T> with
    // indexing using `at` to keep elements C-contiguous
    explicit Matrix(std::vector<std::vector<T>> stacked_data)
    {
        num_rows_ = stacked_data.size();
        num_cols_ = stacked_data[0].size();
        data_.reserve(num_rows_ * num_cols_);
        size_t len_integrity = stacked_data[0].size();

        for (size_t i = 0; i < num_rows_; ++i)
        {
            if (len_integrity != stacked_data[i].size()) {
                throw std::runtime_error("Non-uniform dimensions for matrix");
            }
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

    T &at(size_t row, size_t col)
    {
        return data_[row * num_cols_ + col];
    }

    T operator[](std::tuple<size_t, size_t> idx) {
        return at(std::get<0>(idx), std::get<1>(idx));
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

    std::vector<T> get_row(int row) {
        return std::vector<T>(data_.begin() + row * num_cols_, data_.begin() + row * num_cols_ + num_cols_);
    }

    Matrix<T> operator+(Matrix& other) {
        std::vector<T> c_data;
        if (num_cols_ != other.num_cols() || num_rows_ != other.num_rows()) {
            throw std::runtime_error("Matrices must have same dimension");
        }
        for (size_t i = 0; i < num_rows_; ++i) {
            for (size_t j = 0; j < num_cols_; ++j) {
                c_data.push_back(at(i,j) + other.at(i,j));
            }
        }
        return Matrix<T>(c_data, num_rows_, num_cols_);
    }

    Matrix<T> operator-(Matrix& other) {
        std::vector<T> c_data;
        if (num_cols_ != other.num_cols() || num_rows_ != other.num_rows()) {
            throw std::runtime_error("Matrices must have same dimension");
        }
        for (size_t i = 0; i < num_rows_; ++i) {
            for (size_t j = 0; j < num_cols_; ++j) {
                c_data.push_back(at(i,j) - other.at(i,j));
            }
        }
        return Matrix<T>(c_data, num_rows_, num_cols_);
    }

    Matrix<T> operator*(Matrix& other)
    {
        if (num_cols_ != other.num_rows())
        {
            throw std::runtime_error("Incompatible matrix dimensions");
        }

        std::vector<T> c_data;

        // m indexes over the rows of a
        for (size_t m = 0; m < num_rows_; ++m)
        {
            // p indexes over the cols of b
            for (size_t p = 0; p < other.num_cols(); ++p)
            {
                // n indexes over the cols of a and the rows of b
                T dot_product_result = 0;
                for (size_t n = 0; n < other.num_rows(); ++n)
                {
                    dot_product_result += at(m, n) * other.at(n, p);
                }
                c_data.push_back(dot_product_result);
            }
        }

        return Matrix<T>(c_data, num_rows_, other.num_cols());
    }

    // TODO: Change up function arguments, as `echelon_form(true, false)` is wonky
    // Get row-echelon form using LU decomposition, performed in-place.
    void echelon_form(bool take_upper, bool take_lower) {
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
                // first L element of that row is that factor b,
                // represented by at(i,j) here.
                for (size_t j = 0; j < i; ++j) {
                    sum += at(i, j) * at(j, k);
                }

                at(i, k) = at(i, k) - sum;
            }
            // Calculate the lower triangular matrix L
            // This essentially attempts to find the pivot factor
            // but instead of using it to zero out row elements
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
        // triangular quadrants

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

        // To extract L from the combined Matrix, set diagonal
        // entries to 1 and upper diagonal entries to 0
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

    T det() {
        T result = 1;

        // Copy Matrix to avoid modifying it on call
        // to its determinant
        Matrix<T> mat = *this;

        // Take upper triangular matrix, which is `mat` in
        // echelon form
        mat.echelon_form(true, false);

        // Determinant of an upper triangular matrix
        // is trivially the product of its diagonal entries
        for (size_t i = 0; i < num_rows_; ++i){
            result = result * mat.at(i,i);
        }
        return result;
    }

    std::string to_string() {
        // We want to make sure that the longest column
        // dictates where the braces for the Matrix are

        // Count how many symbols take up each row and
        // whichever has the most decides on the braces
        // placements

        size_t decimals = 1;

        auto get_size = [decimals](T digit) -> size_t {
            size_t size = 0;
            std::string string_digit = std::to_string(digit);
            for (size_t i = 0; i < string_digit.length(); ++i) {
                char entry = string_digit.at(i);
                if (entry != *".") {
                    size++;
                } else if (string_digit.at(i + 1) != *"0") {
                    // Count the decimal
                    size++;
                    // Count `decimals` more digits
                    for (size_t j = 0; j < decimals; ++j) {
                        // (i+1) + j here to denote the first decimal point
                        // as j starts at 0, and i is at the "."
                        size++;
                    }
                        return size;
                }
                else {
                    return size;
                }
            }
        };

        auto get_digits = [get_size](T num, size_t whitespace_allowed) -> T {
            std::string string_digit = std::to_string(num);
            std::string new_digit;

            // Keep popping back until at `decimals` past "."
            for (size_t i = 0; i < whitespace_allowed; ++i) {
                    new_digit += string_digit[i];
                }

            return std::stod(new_digit);
        };


        // Get a row of the largest digits for each column
        std::vector<size_t> minimum_whitespace_per_column;
        for (size_t i = 0; i < num_rows_; ++i) {
            size_t largest_col_elem = 0;
            for (size_t j = 0; j < num_cols_; ++j) {
                T elem = at(j,i);
                size_t elem_size = get_size(elem);
                if (elem_size > largest_col_elem) {
                    largest_col_elem = elem_size;
                }
            }
            minimum_whitespace_per_column.push_back(largest_col_elem);
        }


        std::stringstream ss;
        ss << "Matrix([" << std::endl;

        for (size_t i = 0; i < num_rows_; ++i) {
            ss << "    ";
            ss << "[";

            for (size_t j = 0; j < num_cols_; ++j) {
                T elem = at(i, j);
                size_t whitespace = get_size(elem);
                size_t minimum_whitespace_for_entry = minimum_whitespace_per_column[j];
                size_t whitespace_debt =  minimum_whitespace_for_entry - whitespace;

                for (size_t k = 0; k < whitespace_debt; ++k) {
                    ss << " ";
                }
                T elem_adjusted_for_sigfigs = get_digits(elem, whitespace);
                if (j == num_cols() - 1) {
                    ss << elem_adjusted_for_sigfigs;
                } else {
                    ss << elem_adjusted_for_sigfigs << ", ";
                }
            }
            if (i == num_rows() - 1) {
                ss << "]" << std::endl;
                ss << "])";
            } else {
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
double dot_product(std::vector<T> &v, std::vector<T> &u)
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
#endif