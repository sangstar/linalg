#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/string.h>
#include "linalg.hpp"

namespace nb = nanobind;
using namespace nb::literals;


// TODO: Add documentation for functions and methods
NB_MODULE(linalg, m)
{
    nb::class_<Matrix<float>>(m, "Matrix")
        .def(nb::init<std::vector<std::vector<float>>>())
        .def("num_rows", &Matrix<float>::num_rows)
        .def("num_cols", &Matrix<float>::num_cols)
        .def("data", &Matrix<float>::data)
        .def("__str__", &Matrix<float>::to_string)
        .def("echelon_form", &Matrix<float>::echelon_form, "take_upper"_a = true, "take_lower"_a = false,
             "Get the row-echelon form of Matrix using LU decomposition, performed in-place. "
             "`take_upper=True` returns the upper triangular matrix U, which is Matrix in row-echelon "
             "form, while `take_lower=True` returns the lower triangular matrix L. Note that this "
             "means that take_upper = False and take_lower = False returns a matrix "
             "with both L and U encoded simultaneously in Matrix, which isn't meaningful as they're "
             "simply imputed together just to hold both in one matrix for memory efficiency.");


    m.def("matmul", &matmul<float>, "a"_a, "b"_a);
    m.def("dot_product", &dot_product<float>, "v"_a, "u"_a);
}