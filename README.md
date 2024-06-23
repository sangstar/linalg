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

## Example
`Matrix` taking a list of lists for its constructor allows it to be easily
interoperable with `numpy`.

```python
import numpy as np
import time
a = np.random.rand(10,10)
b = np.random.rand(10,10)

# Also supports casting from `np.array`s with `.tolist()`

a = linalg.Matrix(a.tolist())
b = linalg.Matrix(b.tolist())

start = time.time()
c = a @ b


d = (c @ a) @ b
print(d)
'''
Matrix([
    [ 76.8, 64.2,   57, 59.7, 59.6,  64.4, 46.3,  69.4,  64.4,  65.3],
    [123.8,  103,   92, 96.4, 96.2, 103.1, 74.6, 112.1, 103.1, 104.3],
    [ 90.5, 75.8, 67.3, 70.7, 70.2,  75.8, 54.2,  81.8,  75.5,  76.8],
    [ 92.8, 77.1, 68.8,   72,   72,  77.4, 55.9,  83.9,  77.8,  78.3],
    [105.5,   88, 78.2,   82, 81.8,  88.1, 63.6,  95.4,  88.2,  89.4],
    [110.1, 92.4, 81.8, 85.8, 85.3,  92.4, 66.5,  99.7,  91.7,  93.9],
    [ 84.3, 70.2, 62.2, 65.6, 65.3,    70, 51.2,  76.3,  69.6,  71.1],
    [ 92.3, 76.7, 68.2, 71.6, 71.7,  77.1, 56.1,  83.5,  76.8,  78.2],
    [ 94.6, 79.1, 70.2, 73.5, 73.3,  79.2, 56.9,  85.5,  79.2,  80.4],
    [ 83.1, 69.4, 61.8, 64.6, 64.4,  69.5, 49.8,  75.1,  69.7,  70.5]
])
'''


print(d.det())
'''
9.80322162862115e-07 

Matrix has no inverse
'''

elapsed_time = time.time() - start


print("Elapsed time: %.6f seconds" % elapsed_time)
'''
Elapsed time: 0.000061 seconds
'''
```
