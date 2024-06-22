import linalg

a = linalg.Matrix([[1,3,8],[7,11,12],[1,2,3]])
b = linalg.Matrix([[5,7,56],[12,8,14],[4,5,6]])

c = linalg.matmul(a, b)

print(c)

c.echelon(take_lower = True, take_upper = False)

print(c)