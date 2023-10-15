from linealAlgebra import *
def vertexShader(vertex, modelMatrix):
    vt = [vertex[0],
          vertex[1],
          vertex[2],
          1]
    vt = multiplyVectorMatrix(v=vt, m=modelMatrix)
    vt = [vt[0]/vt[3],
          vt[1]/vt[3],
          vt[2]/vt[3]]
    return vt