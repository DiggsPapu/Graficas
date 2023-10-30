import glm
from OpenGL.GL import *
from OpenGL.GL.shaders import compileProgram, compileShader

class Renderer(object):
    def __init__(self, screen):
        self.screen = screen
        _, _, self.width, self.height = screen.get_rect()

        self.clearColor = [0,0,0]
        
        glEnable(GL_DEPTH_TEST)
        glViewport(0,0,self.width,self.height)

        self.scene = []
        
        self.activeShader = None
    
    def setShaders(self, vertexShader, fragmentShader):
        if vertexShader is not None and fragmentShader is not None:
            self.activeShader = compileProgram(compileShader(vertexShader, GL_VERTEX_SHADER),
                                               compileShader(fragmentShader, GL_FRAGMENT_SHADER) )
        else:
            self.activeShader = None
    def render(self):
        glClearColor(self.clearColor[0],self.clearColor[1],self.clearColor[2],1)
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        
        if self.activeShader is not None:
            glUseProgram(self.activeShader)
        
        for obj in self.scene:
            obj.render()