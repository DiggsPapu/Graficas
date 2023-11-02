import pygame 
from pygame.locals import *
import glm
from Obj import *
from gl import Renderer
from model import Model
from shaders import *

 
width = 960
height = 540

pygame.init()

screen = pygame.display.set_mode((width, height), pygame.OPENGL | pygame.DOUBLEBUF)
clock = pygame.time.Clock()

rend = Renderer(screen)

rend.setShaders(vertex_shader, fragment_shader)

#               POSITIONS                     UVs                   NORMALS
triangleData = [-0.5,   -0.5,   0.0,          0.0,    0.0,          0.0, 0.0, 1.0,
                -0.5,    0.5,   0.0,          0.0,    1.0,          0.0, 0.0, 1.0,
                 0.5,   -0.5,   0.0,          1.0,    0.0,          0.0, 0.0, 1.0,

                -0.5,    0.5,   0.0,          0.0,    1.0,          0.0, 0.0, 1.0,
                0.5,     0.5,   0.0,          1.0,    1.0,          0.0, 0.0, 1.0,
                0.5,    -0.5,   0.0,          1.0,    0.0,          0.0, 0.0, 1.0
                 ]
# astronautData = Obj("./model/astronaut.obj")
# astronaut = Model(astronautData.data)
# astronaut.position.z = -10
# astronaut.scale = glm.vec3(1,1,1)
# astronaut.loadTexture("./textures/pavoreal.jpg")
# rend.scene.append( astronaut )

# astronautData = Obj("./model/toonRocket.obj")
# astronaut = Model(astronautData.data)
# astronaut.position.z = -10
# astronaut.scale = glm.vec3(1,1,1)
# astronaut.loadTexture("./textures/toonRocket.bmp")
# rend.scene.append( astronaut )

carnotaurusData = Obj("./model/Carnotaurus.obj")
carnotaurs = Model(carnotaurusData.data)
carnotaurs.position.z = -10
carnotaurs.scale = glm.vec3(1,1,1)
carnotaurs.loadTexture("./textures/Carnotaurus.bmp")
rend.scene.append( carnotaurs )

# triangleModel = Model(triangleData)
# triangleModel.loadTexture("./textures/pavoreal.jpg")
# triangleModel.position.z = -10
# triangleModel.scale = glm.vec3(5,5,5)

# rend.scene.append( triangleModel )



isRunning = True
while isRunning:
    deltaTime = clock.tick(60) / 1000
    keys = pygame.key.get_pressed()
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            isRunning = False
            
        elif event.type == pygame.KEYDOWN:
            if event.key == K_ESCAPE:
                isRunning = False
                
    if keys[K_d]:
        rend.camPosition.x += 5 * deltaTime
    
    if keys[K_a]:
        rend.camPosition.x -= 5 * deltaTime
    
    if keys[K_w]:
        rend.camPosition.z += 5 * deltaTime
    
    if keys[K_s]:
        rend.camPosition.z -= 5 * deltaTime
    
    if keys[K_q]:
        rend.camPosition.y += 5 * deltaTime
    
    if keys[K_e]:
        rend.camPosition.y -= 5 * deltaTime
        
    if keys[K_RIGHT]:
        if rend.clearColor[0] < 1.0:
            rend.clearColor[0] += deltaTime
    
    if keys[K_LEFT]:
        if rend.clearColor[0] > 0.0:
            rend.clearColor[0] -= deltaTime
    
    if keys[K_UP]:
        if rend.clearColor[1] < 1.0:
            rend.clearColor[1] += deltaTime
    
    if keys[K_DOWN]:
        if rend.clearColor[1] > 0.0:
            rend.clearColor[1] -= deltaTime
    
    if keys[K_z]:
        if rend.clearColor[2] < 1.0:
            rend.clearColor[2] += deltaTime
    
    if keys[K_x]:
        if rend.clearColor[2] > 0.0:
            rend.clearColor[2] -= deltaTime
    carnotaurs.rotation.y += 45 * deltaTime
    rend.elapsedTime += deltaTime
    
    rend.render()
    pygame.display.flip()

pygame.quit()