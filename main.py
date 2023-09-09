from linealAlgebra import *
arr1 = [0,1,2,3,4,5]
arr2 = [5,4,3,2,1,0]
matrix1 = [
    [0,1,2],
    [3,4,5],
    [6,7,8]
]
print(substract(arr1,arr2))
print(add(arr1,arr2))
print(norm(arr1))
print(dotProduct(arr1,arr2))
print(scalarMultVector(2, arr1))
print(scalarMultMatrix(2,matrix1))