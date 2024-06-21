import linalg

a = linalg.Matrix([[1,2,3],[4,5,6]])

b = linalg.Matrix([[1,2,3],[4,5,6], [7,8,9]])

c = linalg.matmul(a, b)

print(a)

print(c)

v1 = [1,2,3]
v2 = [4,5,6]

print(linalg.dot_product(v1, v2))