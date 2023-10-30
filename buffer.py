from OpenGL.GL import *
from numpy import array, float32

class Buffer(object):
    def __init__(self, data):
        
        self.vertBuffer = array(data, dtype=float32)
        
        # Vertex Buffer Object
        self.VBO = glGenBuffers(1)
        
        # Vertex Array Object
        self.VAO = glGenVertexArrays(1)
        
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
            4 * 6,                      # Stride
            ctypes.c_void_p(0))          # Offset
        
        glEnableVertexAttribArray(0)
        
         # Atributo de colores
        glVertexAttribPointer(
            1,                          # Attribute Number
            3,                          # Size
            GL_FLOAT,                   # Type
            GL_FALSE,                   # Is it normalized
            4 * 6,                      # Stride
            ctypes.c_void_p(4*3)             # Offset
        )
        
        glEnableVertexAttribArray(1)
        
        glDrawArrays(GL_TRIANGLES, 0,int( len(self.vertBuffer) / 6))
        