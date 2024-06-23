import linalg

a = linalg.Matrix([[1,3,8],[7,11,12],[1,2,3]])
b = linalg.Matrix([[5,7,56],[12,8,14],[4,5,6]])

# matrix a
print(a)

# row 1, column 2 of matrix a (indexing starts 0)
print(a[1,2])

# a + b
print(a + b)

# a - b
print(a - b)

# matrix a multiplied by matrix b
c = a@b

print(c)

# determinant of matrix c
print(c.det())
