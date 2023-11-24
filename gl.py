# pip install PyGLM
# Libreria de Matematicas compatible
# con OpenGL
import glm

# pip install PyOpenGL
from OpenGL.GL import *
from OpenGL.GL.shaders import compileProgram, compileShader
from numpy import array, float32
import pygame


class Renderer(object):
    def __init__(self, screen):
        self.screen = screen
        _, _, self.width, self.height = screen.get_rect()

        self.clearColor = [0,0,0]
        
        glEnable(GL_DEPTH_TEST)
        glEnable(GL_CULL_FACE)
        glViewport(0,0,self.width,self.height)

        self.elapsedTime = 0.0
        self.target = glm.vec3(0, 0, 0)
        self.camAngle = 0.0
        self.camR = 0.0
        self.intensidad = 0.0
        self.scene = []
        self.activeShader = None
        self.skyboxShader = None
        
        self.filledMode = True
        
        self.dirLight = glm.vec3(-1,0,0)    
        
        # View Matrix
        self.camPosition = glm.vec3(0,0,0)
        self.camRotation = glm.vec3(0,0,0)
        self.viewMatrix = self.getViewMatrix()
        
        # Projection Matrix
        self.projectionMatrix = glm.perspective(glm.radians(60),        # FOV
                                                self.width/self.height, # Aspect Ratio
                                                0.1,                    # Near Plane
                                                1000)                   # Far Plane
        
    def toggleFilledMode(self):
        self.filledMode = not self.filledMode
        
        if self.filledMode:
            glPolygonMode(GL_FRONT, GL_FILL)
        else:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)
    
    def createSkybox(self, textureList, vertexShader, fragmentShader):
        
        skyboxBuffer = [-1.0,  1.0, -1.0,
                        -1.0, -1.0, -1.0,
                        1.0, -1.0, -1.0,
                        1.0, -1.0, -1.0,
                        1.0,  1.0, -1.0,
                        -1.0,  1.0, -1.0,

                        -1.0, -1.0,  1.0,
                        -1.0, -1.0, -1.0,
                        -1.0,  1.0, -1.0,
                        -1.0,  1.0, -1.0,
                        -1.0,  1.0,  1.0,
                        -1.0, -1.0,  1.0,

                        1.0, -1.0, -1.0,
                        1.0, -1.0,  1.0,
                        1.0,  1.0,  1.0,
                        1.0,  1.0,  1.0,
                        1.0,  1.0, -1.0,
                        1.0, -1.0, -1.0,

                        -1.0, -1.0,  1.0,
                        -1.0,  1.0,  1.0,
                        1.0,  1.0,  1.0,
                        1.0,  1.0,  1.0,
                        1.0, -1.0,  1.0,
                        -1.0, -1.0,  1.0,

                        -1.0,  1.0, -1.0,
                        1.0,  1.0, -1.0,
                        1.0,  1.0,  1.0,
                        1.0,  1.0,  1.0,
                        -1.0,  1.0,  1.0,
                        -1.0,  1.0, -1.0,

                        -1.0, -1.0, -1.0,
                        -1.0, -1.0,  1.0,
                        1.0, -1.0, -1.0,
                        1.0, -1.0, -1.0,
                        -1.0, -1.0,  1.0,
                        1.0, -1.0,  1.0]
        
        self.skyboxVertBuffer = array(skyboxBuffer, dtype = float32)
        self.skyboxVBO = glGenBuffers(1)
        self.skyboxVAO = glGenVertexArrays(1)
        
        self.skyboxShader = compileProgram(compileShader(vertexShader, GL_VERTEX_SHADER),
                                               compileShader(fragmentShader, GL_FRAGMENT_SHADER) )
        
        self.skyboxTexture = glGenTextures(1)
        glBindTexture(GL_TEXTURE_CUBE_MAP, self.skyboxTexture)
        for i in range(len(textureList)): # 6 textures
            texture = pygame.image.load(textureList[i])
            textureData = pygame.image.tostring(texture, "RGB", False)
            
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,# Texture Type
                        0,                                  # Positions
                        GL_RGB,                             # Internal Format
                        texture.get_width(),                # Width
                        texture.get_height(),               # Height
                        0,                                  # Border
                        GL_RGB,                             # Format
                        GL_UNSIGNED_BYTE,                   # Type
                        textureData)                        # Data
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE)
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE)
            
    def renderSkyBox(self):
        
        if self.skyboxShader == None:
            return 
        glDepthMask(GL_FALSE)
        
        glUseProgram(self.skyboxShader)
        
        skyboxVM = glm.mat4(glm.mat3(self.viewMatrix))
        
        glUniformMatrix4fv(glGetUniformLocation(self.skyboxShader, "viewMatrix"),
                            1, GL_FALSE, glm.value_ptr(skyboxVM))
            
        glUniformMatrix4fv(glGetUniformLocation(self.skyboxShader, "projectionMatrix"),
                            1, GL_FALSE, glm.value_ptr(self.projectionMatrix))
            
        
        glBindBuffer(GL_ARRAY_BUFFER, self.skyboxVBO)
        glBindVertexArray(self.skyboxVAO)
        
        glBufferData(GL_ARRAY_BUFFER,
                     self.skyboxVertBuffer.nbytes,
                     self.skyboxVertBuffer,
                     GL_STATIC_DRAW)
        
        glVertexAttribPointer(0,                    # Attribute number
                              3,                    # Size
                              GL_FLOAT,             # Type
                              GL_FALSE,             # Is it normalized
                              4 * 3,                # Stride
                              ctypes.c_void_p(0))   # Offset
        
        glEnableVertexAttribArray(0)
        
        glBindTexture(GL_TEXTURE_CUBE_MAP, self.skyboxTexture)
        
        glDrawArrays(GL_TRIANGLES, 0, 36)
        
        glDepthMask(GL_TRUE)
        
    def getViewMatrix(self):
        identity = glm.mat4(1)
        
        translateMat = glm.translate(identity, self.camPosition)
        
        pitch = glm.rotate(identity, glm.radians(self.camRotation.x), glm.vec3(1,0,0) )
        yaw =   glm.rotate(identity, glm.radians(self.camRotation.y), glm.vec3(0,1,0) )
        roll =  glm.rotate(identity, glm.radians(self.camRotation.z), glm.vec3(0,0,1) )
        
        rotationMat = pitch * yaw * roll
        
        camMatrix = translateMat * rotationMat
        
        return glm.inverse(camMatrix)
    
    def setShaders(self, vertexShader, fragmentShader):
        if vertexShader is not None and fragmentShader is not None:
            self.activeShader = compileProgram(compileShader(vertexShader, GL_VERTEX_SHADER),
                                               compileShader(fragmentShader, GL_FRAGMENT_SHADER) )
        else:
            self.activeShader = None
    
    def update(self):
        # self.viewMatrix = self.getViewMatrix()
        self.viewMatrix = glm.lookAt(self.camPosition, self.target, glm.vec3(0,1,0))
        
    def render(self):
        glClearColor(self.clearColor[0],self.clearColor[1],self.clearColor[2], 1)
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        
        self.renderSkyBox()
        
        if self.activeShader is not None:
            glUseProgram(self.activeShader)
            
            glUniformMatrix4fv(glGetUniformLocation(self.activeShader, "viewMatrix"),
                               1, GL_FALSE, glm.value_ptr(self.viewMatrix))
            
            glUniformMatrix4fv(glGetUniformLocation(self.activeShader, "projectionMatrix"),
                               1, GL_FALSE, glm.value_ptr(self.projectionMatrix))
            
            glUniform1f( glGetUniformLocation(self.activeShader, "intensidad"), self.intensidad)
            
            glUniform1f( glGetUniformLocation(self.activeShader, "time"), self.elapsedTime)
            
            glUniform3fv( glGetUniformLocation(self.activeShader, "dirLight"), 1, glm.value_ptr(self.dirLight))
        
        for obj in self.scene:
            if self.activeShader is not None:
                glUniformMatrix4fv(glGetUniformLocation(self.activeShader, "modelMatrix"),
                               1, GL_FALSE, glm.value_ptr(obj.getModelMatrix()))
        
            
            obj.render()