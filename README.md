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

a = linalg.Matrix([[1,3,8],[7,11,12],[1,2,3]])
b = linalg.Matrix([[5,7,56],[12,8,14],[4,5,6]])

c = linalg.matmul(a, b)

print(c)

'''
Matrix([
    [73, 71, 146],
    [215, 197, 618],
    [41, 38, 102]
])
'''

c.echelon()

print(c)

'''
Matrix([
    [73, 71, 146],
    [0, -12.1096, 188],
    [0, 0, -9.13576]
])
'''
```
