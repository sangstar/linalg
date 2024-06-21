#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/string.h>
#include "linalg.hpp"

namespace nb = nanobind;
using namespace nb::literals;

NB_MODULE(linalg, m)
{
    nb::class_<Matrix<float>>(m, "Matrix")
        .def(nb::init<std::vector<std::vector<float>>>())
        .def("num_rows", &Matrix<float>::num_rows)
        .def("num_cols", &Matrix<float>::num_cols)
        .def("data", &Matrix<float>::data)
        .def("__str__", &Matrix<float>::to_string);

    m.def("matmul", &matmul<float>, "a"_a, "b"_a);
    m.def("dot_product", &dot_product<float>, "v"_a, "u"_a);
}