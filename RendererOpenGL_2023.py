import pygame 
from pygame.locals import *
import glm
from Obj import *
from gl import Renderer
from model import Model
from shaders import *
from linearalgebra import *
 
width = 960
height = 540

pygame.init()

screen = pygame.display.set_mode((width, height), pygame.OPENGL | pygame.DOUBLEBUF)
clock = pygame.time.Clock()

rend = Renderer(screen)


vert_s = vertex_shader
frag_s = fragment_shader0
rend.setShaders(vertex_shader, fragment_shader0)
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
counter_direction = 0
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
            if event.key == pygame.K_SPACE:
                rend.toggleFilledMode()
            # Direction of light
            if event.key == K_l:
                counter_direction+=1
                o = [[1,0,0],[-1,0,0],
                    [0,1,0],[0,-1,0],
                    [0,0,1],[0,0,1]
                    ]
                dirl = o[counter_direction%6]
                rend.dirLight = glm.vec3(dirl[0], dirl[1], dirl[2])
            if event.key == K_o:
                if rend.intensidad<1:
                    rend.intensidad +=0.1
            if event.key == K_p:
                if rend.intensidad>0:
                    rend.intensidad -=0.1
                
        
                
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
    
    # Vertex shader
    if keys[K_6]:
        vert_s = twistDeformationShader
        rend.setShaders(vert_s, frag_s)
        
    if keys[K_7]:        
        vert_s = fragmentation_shader
        rend.setShaders(vert_s, frag_s)  
    # Fragment shader
        
    if keys[K_0]: 
        vert_s = vertex_shader
        frag_s = fragment_shader     
        rend.setShaders(vert_s, frag_s)
    
    if keys[K_1]: 
        frag_s = inverse_shader   
        rend.setShaders(vert_s, frag_s)
        
    if keys[K_2]:
        frag_s = temperature_shader
        rend.setShaders(vert_s, temperature_shader)
    
    if keys[K_3]:
        frag_s = wave_draw_lines
        rend.setShaders(vert_s, wave_draw_lines)
    if keys[K_4]:        
        frag_s = fragmentation_shader1
        rend.setShaders(vert_s, frag_s)
    carnotaurs.rotation.y += 45 * deltaTime        
    rend.elapsedTime += deltaTime
    
    
    rend.update()
    rend.render()
    pygame.display.flip()

pygame.quit()