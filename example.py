import linalg

print(linalg.dot_product([1,2,3], [4,5,6]))

a = linalg.Matrix([[1,3,8],[7,11,12],[1,2,3]])
b = linalg.Matrix([[5,7,56],[12,8,14],[4,5,6]])


print(a)


print(a[1,2])

print(a + b)

print(a - b)

c = a@b

print(c)

c.echelon_form()

print(c)

print(c.det())

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

print(d.det())

elapsed_time = time.time() - start


print("Elapsed time: %.6f seconds" % elapsed_time)

a = np.random.rand(50304,512)
mat = linalg.Matrix(a.tolist())

reduced = linalg.reduce_with_fastmap(mat, 2)
print("Reduced: ",reduced)
