# linalg

A Python module implemented in C++ to perform linear algebra.

## Quickstart

Configure the provided `CMakeLists.txt` and build the module:

```bash
cmake -S . -B build
cmake --build build
```

The shared object file will be available in the root directory and can be
imported directly in to Python.

```python
import linalg

a = linalg.Matrix([[1,2,3],[4,5,6]])
b = linalg.Matrix([[1,2,3],[4,5,6], [7,8,9]])
c = linalg.matmul(a, b)


print(c)
```
