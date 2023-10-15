def subtract(arr1, arr2):
    if (len(arr1) == len(arr2)):
        return [arr1[x]-arr2[x] for x in range(0,len(arr1))]
    return None
def add(arr1, arr2):
    if (len(arr1) == len(arr2)):
        return [arr1[x]+arr2[x] for x in range(0,len(arr1))]
    return None
def norm(arr1):
    return ((sum([i**2 for i in arr1])))**(1/2)
def normalize(arr1):
    return scalarMultVector(1/norm(arr1),arr1)
def dotProduct(arr1, arr2):
    if (len(arr1) == len(arr2)):
        return sum(arr1[x]*arr2[x] for x in range(0,len(arr1)))
    return None
def scalarMultVector(val, arr1):
    return [val*arr1[x] for x in range(0,len(arr1))]
def multiplyVectorVector(arr1,arr2):
    if len(arr1) == len(arr2):
        return [arr1[i]*arr2[i] for i in range(arr1)]
    return None
def multiplyVectorMatrix(v, m):
    result = []
    for x in range(len(m)):
        value = 0
        for y in range(len(v)):
            value+=m[x][y]*v[y]
        result.append(value)
    return result
        
def scalarMultMatrix(val, matrix):
    result = []
    for x in range(0,len(matrix)):
        row = []
        for y in range(0, len(matrix[x])):
            row.append(val*matrix[x][y])
        result.append(row) 
    return result
def tupleToArray(tuple):
    return [tuple[x] for x in range(0,len(tuple))]
def negArray(array):
    return [array[i]*-1 for i in range(0,len(array))]

def crossProduct(vec1, vec2):
    if len(vec1) != len(vec2):
        raise ValueError("Input vectors must have the same length.")
    n = len(vec1)
    if n < 3:
        raise ValueError("The cross product is only defined for vectors of length at least 3.")
    result = [0] * n
    for i in range(n):
        next_i = (i + 1) % n
        next_next_i = (i + 2) % n
        result[i] = vec1[next_i] * vec2[next_next_i] - vec1[next_next_i] * vec2[next_i]
    return result

def matrixMatrixMultiplication(m1, m2):
    if len(m1)!=len(m2[0]):
        raise ValueError("The matrix matrix multiplication cant be done, not same size.")
    result = []
    for z in range(len(m1)):
        val = []
        for x in range(len(m1[0])):
            d = 0
            for y in range(len(m2)):
                d+=m1[z][y]*m2[y][x]
            val.append(d)
        result.append(val)
    return result
def v4v3(v):
    return [v[x] for x in range(3)]


def multiplicar_matrices(matriz1, matriz2):
    filas_matriz1 = len(matriz1)
    columnas_matriz1 = len(matriz1[0])
    filas_matriz2 = len(matriz2)
    columnas_matriz2 = len(matriz2[0])

    # Verificar si las matrices se pueden multiplicar
    if columnas_matriz1 != filas_matriz2:
        print("No se pueden multiplicar las matrices.")
        return None

    # Crear una matriz de resultado con las dimensiones adecuadas
    matriz_resultado = [[0 for y in range(columnas_matriz2)] for x in range(filas_matriz1)]

    # Realizar la multiplicación de matrices
    for i in range(filas_matriz1):
        for j in range(columnas_matriz2):
            for k in range(columnas_matriz1):
                matriz_resultado[i][j] += matriz1[i][k] * matriz2[k][j]

    return matriz_resultado

def multiplicar_matriz_vector(matriz,vector):
    filas_matriz = len(matriz)
    columnas_matriz = len(matriz[0])
    filas_vector = len(vector)

    # Verificar si las matrices se pueden multiplicar
    if columnas_matriz!= filas_vector:
        print("No se puede multiplicar la matriz por el vector.")
        return None

    # Crear un vector de resultado con las dimensiones adecuadas
    vector_resultado = [0 for x in range(filas_matriz)]

    # Realizar la multiplicación matriz-vector
    for i in range(filas_matriz):
        for k in range(columnas_matriz):
            vector_resultado[i] += matriz[i][k] * vector[k]

    return vector_resultado
        
    