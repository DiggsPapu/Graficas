import pygame 
from pygame.locals import *

from gl import Renderer
from buffer import Buffer
from shaders import *


width = 960
height = 540

pygame.init()

screen = pygame.display.set_mode((width, height), pygame.OPENGL | pygame.DOUBLEBUF)

clock = pygame.time.Clock()

rend = Renderer(screen)

rend.setShaders(vertex_shader, fragment_shader)

#           POSITIONS                   COLORS
triangle = [-0.5,   -0.5,   +0.0,   1.0,    0.0,    0.0,
            +0.0,   +0.5,   +0.0,   0.0,    1.0,    0.0,
            +0.5,   -0.5,   +0.0,    0.0,    0.0,    1.0 ]
rend.scene.append( Buffer(triangle) )

isRunning = True
while isRunning:
    keys = pygame.key.get_pressed()
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            isRunning = False
            
        elif event.type == pygame.KEYDOWN:
            if event.key == K_ESCAPE:
                isRunning = False
    
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
    deltaTime = clock.tick(60) / 1000
    
    if keys[K_z]:
        if rend.clearColor[2] < 1.0:
            rend.clearColor[2] += deltaTime
    
    if keys[K_x]:
        if rend.clearColor[2] > 0.0:
            rend.clearColor[2] -= deltaTime
    deltaTime = clock.tick(60) / 1000
    rend.render()
    pygame.display.flip()

pygame.quit()