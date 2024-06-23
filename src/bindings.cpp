#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/tuple.h>
#include "linalg.hpp"

namespace nb = nanobind;
using namespace nb::literals;


// TODO: Add documentation for functions and methods
NB_MODULE(linalg, m)
{
    nb::class_<Matrix<double>>(m, "Matrix")
        .def(nb::init<std::vector<std::vector<double>>>())
        .def("num_rows", &Matrix<double>::num_rows)
        .def("num_cols", &Matrix<double>::num_cols)
        .def("data", &Matrix<double>::data)
        .def("__str__", &Matrix<double>::to_string)
        .def("__add__", &Matrix<double>::operator+, "other"_a)
        .def("__sub__", &Matrix<double>::operator-, "other"_a)
        .def("__matmul__", &Matrix<double>::operator*, "other"_a)
        .def("__getitem__", &Matrix<double>::operator[], "tuple"_a)
        .def("echelon_form", &Matrix<double>::echelon_form, "take_upper"_a = true, "take_lower"_a = false,
         "Get the row-echelon form of Matrix using LU decomposition, performed in-place. "
         "`take_upper=True` returns the upper triangular matrix U, which is Matrix in row-echelon "
         "form, while `take_lower=True` returns the lower triangular matrix L. Note that this "
         "means that take_upper = False and take_lower = False returns a matrix "
         "with both L and U encoded simultaneously in Matrix, which isn't meaningful as they're "
         "simply imputed together just to hold both in one matrix for memory efficiency.")
        .def("det", &Matrix<double>::det, "Gets the determinant of Matrix");


    m.def("dot_product", &dot_product<double>, "v"_a, "u"_a);
}