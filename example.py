import linalg

a = linalg.Matrix([[1,2,3],[4,5,6]])
b = linalg.Matrix([[1,2,3],[4,5,6], [7,8,9]])

c = linalg.matmul(a, b)


print(c)