from OpenGL.GL import *
import glm
from numpy import array, float32

import pygame


class Model(object):
    def __init__(self, data):
        
        self.vertBuffer = array(data, dtype=float32)
        
        # Vertex Buffer Object
        self.VBO = glGenBuffers(1)
        
        # Vertex Array Object
        self.VAO = glGenVertexArrays(1)
        
        self.position = glm.vec3(0,0,0)
        self.rotation = glm.vec3(0,0,0)
        self.scale = glm.vec3(1,1,1)
     
     
    def loadTexture(self, textureName):
        self.textureSurface = pygame.image.load(textureName)
        self.textureData    = pygame.image.tostring(self.textureSurface, "RGB", True)
        self.textureBuffer = glGenTextures(1)
        
        
    def getModelMatrix(self):
        identity = glm.mat4(1)
        
        translateMat = glm.translate(identity, self.position)
        
        pitch = glm.rotate(identity, glm.radians(self.rotation.x), glm.vec3(1,0,0) )
        yaw =   glm.rotate(identity, glm.radians(self.rotation.y), glm.vec3(0,1,0) )
        roll =  glm.rotate(identity, glm.radians(self.rotation.z), glm.vec3(0,0,1) )
        
        rotationMat = pitch * yaw * roll
        
        scaleMat = glm.scale(identity, self.scale)
        
        return translateMat * rotationMat * scaleMat
    
    
    def render(self):
        
        # Atamos los buffers del object a la GPU
        glBindBuffer(GL_ARRAY_BUFFER, self.VBO)
        glBindVertexArray(self.VAO)
        
        # Especificar la informacion de vertices
        glBufferData(
            GL_ARRAY_BUFFER,        # Buffer ID
            self.vertBuffer.nbytes, # Buffer Size en Bytes
            self.vertBuffer,        # Buffer data
            GL_STATIC_DRAW)         # Usagge
        
        # Atributos
        # Especificar que representa el contenido del vertice
        
        # Atributo de posiciones
        glVertexAttribPointer(
            0,                          # Attribute Number
            3,                          # Size
            GL_FLOAT,                   # Type
            GL_FALSE,                   # Is it normalized
            4 * 8,                      # Stride
            ctypes.c_void_p(0))          # Offset
        
        glEnableVertexAttribArray(0)
        
         # Atributo de colores
        glVertexAttribPointer(
            1,                          # Attribute Number
            3,                          # Size
            GL_FLOAT,                   # Type
            GL_FALSE,                   # Is it normalized
            4 * 8,                      # Stride
            ctypes.c_void_p(4*3)             # Offset
        )
        
        glEnableVertexAttribArray(1)
        
         # Atributo de coordenadas de textura
        glVertexAttribPointer(
            2,                          # Attribute Number
            2,                          # Size
            GL_FLOAT,                   # Type
            GL_FALSE,                   # Is it normalized
            4 * 8,                      # Stride
            ctypes.c_void_p(4*6)             # Offset
        )
        
        glEnableVertexAttribArray(2)
        
        # Activar la textura
        glActiveTexture( GL_TEXTURE0 )
        glBindTexture( GL_TEXTURE_2D, self.textureBuffer)
        glTexImage2D(   GL_TEXTURE_2D,                     # Texture Type
                        0,                                  # Positions
                        GL_RGB,                             # Internal Format
                        self.textureSurface.get_width(),    # Width
                        self.textureSurface.get_height(),   # Height
                        0,                                  # Border
                        GL_RGB,                             # Format
                        GL_UNSIGNED_BYTE,                   # Type
                        self.textureData)                   # Data
        glGenerateTextureMipmap(self.textureBuffer)
        
        
        glDrawArrays(GL_TRIANGLES, 0,int( len(self.vertBuffer) / 8))
        