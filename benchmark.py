import numpy as np
import linalg
import time

a = np.random.rand(6,6)
b = np.random.rand(400, 400)


linalg_a = linalg.Matrix(a.tolist())
linalg_b = linalg.Matrix(b.tolist())

start = time.time()
a @ b
finish = time.time()

print(finish - start)


start = time.time()
linalg_a.det()
finish = time.time()

print(finish - start)


