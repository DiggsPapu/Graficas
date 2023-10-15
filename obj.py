from figures import Triangle
from Shaders import * 
from materials import *
from linealAlgebra import *
from math import cos, sin, pi
class Model(object):
    def __init__(self, filename, translate = (0,0,0), rotate = (0,0,0), scale = (1,1,1), material = Material()):
        # Asumiendo que el archivo es un formato .obj
        with open(filename,"r") as file:
            self.lines = file.read().splitlines()

        # Se crean los contenedores de los datos del modelo.
        self.vertices = []
        self.texcoords = []
        self.normals = []
        self.faces = []
        self.transformedVerts = []
        self.glModelMatrix(translate=translate,rotate=rotate,scale=scale)
        # Por cada linea en el archivo
        for line in self.lines:
            # Si la linea no cuenta con un prefijo y un valor,
            # seguimos a la siguiente linea
            try:
                prefix, value = line.split(" ", 1)
            except:
                continue

            # Dependiendo del prefijo, parseamos y guardamos la informacion
            # en el contenedor correcto

            if prefix == "v": # Vertices
                self.vertices.append( list(map(float, value.split(" "))) )
            elif prefix == "vt": # Texture Coordinates
                self.texcoords.append( list(map(float, value.split(" "))) )
            elif prefix == "vn": # Normals
                self.normals.append( list(map(float, value.split(" "))) )
            elif prefix == "f": # Faces
                self.faces.append([list(map(int, vert.split("/"))) for vert in value.split(" ") ] )
        self.material = material
        self.primitives = []
        self.convertToRayTracer()
    def convertToRayTracer(self):
        # Para cada cara del modelo
        for face in self.faces:
            vertCount = len(face)
            # Obtenemos los vértices de la cara actual.
            v0 = self.vertices[face[0][0] - 1]
            v1 = self.vertices[face[1][0] - 1]
            v2 = self.vertices[face[2][0] - 1]
            if vertCount == 4:
                v3 = self.vertices[face[3][0] - 1]
                
            if vertexShader:
                v0 = vertexShader(v0,self.finalMatrix)
                v1 = vertexShader(v1,self.finalMatrix)
                v2 = vertexShader(v2,self.finalMatrix)

                if vertCount == 4:
                    v3 = vertexShader(v3,self.finalMatrix)
            # Agregar cada vértice transformado al listado de vértices.
            self.transformedVerts.append(v0)
            self.transformedVerts.append(v1)
            self.transformedVerts.append(v2)
            if vertCount == 4:
                self.transformedVerts.append(v0)
                self.transformedVerts.append(v2)
                self.transformedVerts.append(v3)
        # Esta función construye las primitivas de acuerdo con la
        # opción de primitiva actual. De momento solo hay para triángulos
        for i in range(0, len(self.transformedVerts), 3):
            # Un triángulo contará con las posiciones de sus vértices y
            # y sus UVs, seguidos uno tras otro.
            self.primitives.append(Triangle (vertices=[self.transformedVerts[i],
                                           self.transformedVerts[i + 1],
                                           self.transformedVerts[i + 2]],
                                    material=self.material))

    def glModelMatrix(self, translate = (0,0,0), rotate = (0,0,0), scale = (1,1,1)):
        # Convertir a radianes
        pitch =rotate[0]*pi/180
        yaw =rotate[1]* pi/180
        roll =rotate[2]* pi/180
        # Creamos la matriz de rotación para cada eje.
        pitchMat = [[1,0,0,0],
                    [0,cos(pitch),-sin(pitch),0],
                    [0,sin(pitch),cos(pitch),0],
                    [0,0,0,1]]
        yawMat = [[cos(yaw),0,sin(yaw),0],
                    [0,1,0,0],
                    [-sin(yaw),0,cos(yaw),0],
                    [0,0,0,1]]
        rollMat = [[cos(roll),-sin(roll),0,0],
                    [sin(roll),cos(roll),0,0],
                    [0,0,1,0],
                    [0,0,0,1]]
        # Matrix de rotación
        rotMat = matrixMatrixMultiplication(matrixMatrixMultiplication(pitchMat,yawMat),rollMat)
        # Matriz de traslación
        translation = [[1,0,0,translate[0]],
                        [0,1,0,translate[1]],
                        [0,0,1,translate[2]],
                        [0,0,0,1]]
        # Matriz de escala
        scaleMat = [[scale[0],0,0,0],
                    [0,scale[1],0,0],
                    [0,0,scale[2],0],
                    [0,0,0,1]]
        
        # Se multiplican las tres para obtener la matriz del objeto final
        self.finalMatrix = matrixMatrixMultiplication(matrixMatrixMultiplication(translation,rotMat),scaleMat)
   