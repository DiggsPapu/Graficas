import numpy as np
from support import *
from math import pi, sin, cos
from mathLibrary import *
from model import Model

POINTS = 0
LINES = 1
TRIANGLES = 2
QUADS = 3
class Renderer(object):
    def __init__(self, width, height):
        self.width = width
        self.height = height

        self.glClearColor(0,0,0)
        self.glClear()

        self.glColor(1,1,1)

        self.objects = []

        self.vertexShader = None
        self.fragmentShader = None

        self.primitiveType = TRIANGLES

        self.activeTexture = None

    
    def glClearColor(self, r, g, b):
        self.clearColor = color(r,g,b)


    def glColor(self, r, g, b):
        self.currColor = color(r,g,b)


    def glClear(self):
        self.pixels = [[self.clearColor for y in range(self.height)]
                       for x in range(self.width)]
        self.zbuffer = [[float('inf') for y in range(self.height)]
                       for x in range(self.width)]


    def glPoint(self, x, y, clr = None):
        if (0 <= x < self.width) and (0 <= y < self.height):
            self.pixels[x][y] = clr or self.currColor


    def glTriangle(self, A, B, C, vtA, vtB, vtC):
        minX = round(min(A[0], B[0], C[0]))
        maxX = round(max(A[0], B[0], C[0]))
        minY = round(min(A[1], B[1], C[1]))
        maxY = round(max(A[1], B[1], C[1]))
        for x in range(minX, maxX + 1):
            for y in range(minY, maxY + 1):
                if (0 <= x < self.width) and (0 <= y < self.height):
                    P = (x,y)
                    bCoords = barycentricCoords(A, B, C, P)
                    if bCoords != None:
                        u, v, w = bCoords
                        z = u * A[2] + v * B[2] + w * C[2]
                        if z < self.zbuffer[x][y]:
                            self.zbuffer[x][y] = z
                            uvs = (u * vtA[0] + v * vtB[0] + w * vtC[0],
                                   u * vtA[1] + v * vtB[1] + w * vtC[1])
                            if self.fragmentShader != None:
                                colorP = self.fragmentShader(texCoords = uvs,texture = self.activeTexture)
                                self.glPoint(x, y, color(colorP[0], colorP[1], colorP[2]))
                            else:
                                self.glPoint(x, y)


    def glPrimitiveAssembly(self, tVerts, tTexCoords):
        primitives = [ ]

        if self.primitiveType == TRIANGLES:
            for i in range(0, len(tVerts), 3):

                triangle = [ ]
                # Verts
                triangle.append( tVerts[i] )
                triangle.append( tVerts[i + 1] )
                triangle.append( tVerts[i + 2] )

                # TexCoords
                triangle.append( tTexCoords[i] )
                triangle.append( tTexCoords[i + 1] )
                triangle.append( tTexCoords[i + 2] )

                primitives.append(triangle)

        return primitives


    def glModelMatrix(self, translate = (0,0,0), rotate = (0,0,0), scale = (1,1,1)):
        pitchM = [[1,0,0,0],[0,cos(rotate[0]*pi/180),-sin(rotate[0]*pi/180),0],[0,sin(rotate[0]*pi/180),cos(rotate[0]*pi/180),0],[0,0,0,1]]
        yawM = [[cos(rotate[1]*pi/180),0,sin(rotate[1]*pi/180),0],[0,1,0,0],[-sin(rotate[1]*pi/180),0,cos(rotate[1]*pi/180),0],[0,0,0,1]]
        rollM = [[cos(rotate[2]*pi/180),-sin(rotate[2]*pi/180),0,0],[sin(rotate[2]*pi/180),cos(rotate[2]*pi/180),0,0],[0,0,1,0],[0,0,0,1]]
        transM = [[1,0,0,translate[0]],[0,1,0,translate[1]],[0,0,1,translate[2]],[0,0,0,1]]
        rotM = dotProductMatrixMatrix(dotProductMatrixMatrix(pitchM,yawM), rollM)
        scaleM = [[scale[0],0,0,0],[0,scale[1],0,0],[0,0,scale[2],0],[0,0,0,1]]
        return dotProductMatrixMatrix(dotProductMatrixMatrix(transM,rotM),scaleM)


    def glRotationMatrix(self, pitch = 0, yaw = 0, roll = 0):
        pitch *= pi/180
        yaw *= pi/180
        roll *= pi/180
        pitchM = [[1,0,0,0],[0,cos(pitch),-sin(pitch),0],[0,sin(pitch),cos(pitch),0],[0,0,0,1]]
        yawM = [[cos(yaw),0,sin(yaw),0],[0,1,0,0],[-sin(yaw),0,cos(yaw),0],[0,0,0,1]]
        rollM = [[cos(roll),-sin(roll),0,0],[sin(roll),cos(roll),0,0],[0,0,1,0],[0,0,0,1]]
        result = dotProductMatrixMatrix(dotProductMatrixMatrix(pitchM,yawM),rollM)
        pitchMat = np.matrix([[1,0,0,0],[0,cos(pitch),-sin(pitch),0],[0,sin(pitch),cos(pitch),0],[0,0,0,1]])
        yawMat = np.matrix([[cos(yaw),0,sin(yaw),0],[0,1,0,0],[-sin(yaw),0,cos(yaw),0],[0,0,0,1]])
        rollMat = np.matrix([[cos(roll),-sin(roll),0,0],[sin(roll),cos(roll),0,0],[0,0,1,0],[0,0,0,1]])
        return pitchMat * yawMat * rollMat

    def glLine(self, v0, v1, clr = None):
        x0 = int(v0[0])
        x1 = int(v1[0])
        y0 = int(v0[1])
        y1 = int(v1[1])
        if x0 == x1 and y0 == y1:
            self.glPoint(x0,y0)
            return
        dy = abs(y1 - y0)
        dx = abs(x1 - x0)
        steep = dy > dx
        if steep:
            x0, y0 = y0,x0
            x1, y1 = y1,x1
        if x0 > x1:
            x0, x1 = x1, x0
            y0, y1 = y1, y0
        dy = abs(y1 - y0)
        dx = abs(x1 - x0)
        offset = 0
        limit = 0.5
        m = dy/dx
        y = y0
        for x in range(x0, x1 + 1):
            if steep:
                self.glPoint(y, x, clr or self.currColor)
            else:
                self.glPoint(x, y, clr or self.currColor)
            offset += m
            if offset >= limit:
                if y0 < y1:
                    y += 1
                else:
                    y -= 1
                limit += 1
    def glLoadModel(self, filename, textureName, translate = (0,0,0), rotate = (0,0,0), scale = (1,1,1)):
        model = Model(filename, translate, rotate, scale)
        model.LoadTexture(textureName)
        self.objects.append( model )
    def glRender(self):
        transformedVerts = []
        texCoords = []
        for model in self.objects:
            self.activeTexture = model.texture
            mMat = self.glModelMatrix(model.translate, model.rotate, model.scale)
            for face in model.faces:
                vertCount = len(face)
                v0 = model.vertices[ face[0][0] - 1]
                v1 = model.vertices[ face[1][0] - 1]
                v2 = model.vertices[ face[2][0] - 1]
                if vertCount == 4:
                    v3 = model.vertices[ face[3][0] - 1]
                if self.vertexShader:
                    v0 = self.vertexShader(v0, modelMatrix = mMat)
                    v1 = self.vertexShader(v1, modelMatrix = mMat)
                    v2 = self.vertexShader(v2, modelMatrix = mMat)
                    if vertCount == 4:
                        v3 = self.vertexShader(v3, modelMatrix = mMat)
                transformedVerts.append(v0)
                transformedVerts.append(v1)
                transformedVerts.append(v2)
                if vertCount == 4:
                    transformedVerts.append(v0)
                    transformedVerts.append(v2)
                    transformedVerts.append(v3)
                vt0 = model.texcoords[face[0][1] - 1]
                vt1 = model.texcoords[face[1][1] - 1]
                vt2 = model.texcoords[face[2][1] - 1]
                if vertCount == 4:
                    vt3 = model.texcoords[face[3][1] - 1]
                texCoords.append(vt0)
                texCoords.append(vt1)
                texCoords.append(vt2)
                if vertCount == 4:
                    texCoords.append(vt0)
                    texCoords.append(vt2)
                    texCoords.append(vt3)
        primitives = self.glPrimitiveAssembly(transformedVerts, texCoords)       
        for prim in primitives:
            if self.primitiveType ==  TRIANGLES:
                self.glTriangle(prim[0], prim[1], prim[2],
                                prim[3], prim[4], prim[5])
        


    def glFinish(self, filename):
        with open(filename, "wb") as file:
            file.write(char("B"))
            file.write(char("M"))
            file.write(dword(14 + 40 + (self.width * self.height * 3)))
            file.write(dword(0))
            file.write(dword(14 + 40))
            file.write(dword(40))
            file.write(dword(self.width))
            file.write(dword(self.height))
            file.write(word(1))
            file.write(word(24))
            file.write(dword(0))
            file.write(dword(self.width * self.height * 3))
            file.write(dword(0))
            file.write(dword(0))
            file.write(dword(0))
            file.write(dword(0))
            for y in range(self.height):
                for x in range(self.width):
                    file.write(self.pixels[x][y])
        self.activeTexture = None