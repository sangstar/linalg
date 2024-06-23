# linalg

A Python module implemented in C++ to perform linear algebra calculations.

## Quickstart

Configure the provided `CMakeLists.txt` and build the module:

```bash
cmake -S . -B build
cmake --build build
```

The shared object file will be available in the root directory and can be
imported directly in to Python.

## Overview
The principle object in `linalg` is `Matrix`. It is constructed by passing a
Python list of uniform length lists, and is stored C-contiguously.

```python
import linalg
a = linalg.Matrix([[1,3,8],[7,11,12],[1,2,3]])
```

`linalg` supports a number of useful matrix operations, as exemplified here:

```python
import linalg

a = linalg.Matrix([[1,3,8],[7,11,12],[1,2,3]])
b = linalg.Matrix([[5,7,56],[12,8,14],[4,5,6]])

print(a)
'''
Matrix([
    [1,  3,  8],
    [7, 11, 12],
    [1,  2,  3]
])
'''

print(a[1,2])
'''
12.0
'''

print(a + b)
'''
Matrix([
    [ 6, 10, 64],
    [19, 19, 26],
    [ 5,  7,  9]
])
'''

print(a - b)
'''
Matrix([
    [-4, -4, -48],
    [-5,  3,  -2],
    [-3, -3,  -3]
])
'''

c = a @ b

print(c)
'''
Matrix([
    [ 73,  71, 146],
    [215, 197, 618],
    [ 41,  38, 102]
])
'''

c.echelon_form()

print(c)
'''
Matrix([
    [73,    71,  146],
    [ 0, -12.1,  188],
    [ 0,     0, -9.1]
])
'''

print(c.det())
'''
8076.009765625
'''

```
