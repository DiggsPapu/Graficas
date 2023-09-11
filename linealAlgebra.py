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