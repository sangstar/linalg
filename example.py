import linalg

a = linalg.Matrix([[1,3,8],[7,11,12],[1,2,3]])
b = linalg.Matrix([[5,7,56],[12,8,14],[4,5,6]])

print(a[0,1])
print(a + b)
c = a@b


print(c)

print(c.det())

print(c)
