from math import isclose
def printMatrix(M1):
    for r in M1:
        print(r)
def dotProductMatrixMatrix(M1, M2):
    if len(M1) != 4 or len(M1[0]) != 4 or len(M2) != 4 or len(M2[0]) != 4:
        raise ValueError("Both matrices must be 4x4 matrices")
    result = [[0 for _ in range(4)] for _ in range(4)]
    for i in range(4):
        for j in range(4):
            for k in range(4):
                result[i][j] += M1[i][k] * M2[k][j]
    return result
def dotProductMatrixVertex(matrix, vector):
    if len(matrix) != 4 or len(matrix[0]) != 4 or len(vector) != 4:
        raise ValueError("Matrix must be 4x4 and vector must be 4-dimensional")
    result = [0, 0, 0, 0]
    for i in range(4):
        for j in range(4):
            result[i] += matrix[i][j] * vector[j]
    return result
def barycentricCoords(A, B, C, P):
    areaPCB = abs((P[0]*C[1] + C[0]*B[1] + B[0]*P[1]) - (P[1]*C[0] + C[1]*B[0] + B[1]*P[0]))
    areaACP = abs((A[0]*C[1] + C[0]*P[1] + P[0]*A[1]) - (A[1]*C[0] + C[1]*P[0] + P[1]*A[0]))
    areaABP = abs((A[0]*B[1] + B[0]*P[1] + P[0]*A[1]) - (A[1]*B[0] + B[1]*P[0] + P[1]*A[0]))
    areaABC = abs((A[0]*B[1] + B[0]*C[1] + C[0]*A[1]) - (A[1]*B[0] + B[1]*C[0] + C[1]*A[0]))
    if areaABC == 0:
        return None
    u = areaPCB/areaABC
    v = areaACP/areaABC
    w = areaABP/areaABC
    if 0<=u<=1 and 0<=v<=1 and 0<=w<=1 and isclose(u+v+w, 1.0):
        return (u, v, w)
    else:
        return None
