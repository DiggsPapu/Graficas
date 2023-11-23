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

skyboxTextures = ["./textures/skybox/px.png",
                  "./textures/skybox/nx.png",
                  "./textures/skybox/py.png",
                  "./textures/skybox/ny.png",
                  "./textures/skybox/pz.png",
                  "./textures/skybox/nz.png"]

rend.createSkybox(skyboxTextures, skybox_vertex_shader, skybox_fragment_shader)
vert_s = vertex_shader
frag_s = fragment_shader0
rend.setShaders(vertex_shader, fragment_shader0)

# Carnotaurus
carnotaurusData = Obj("./model/Carnotaurus.obj")
carnotaurus = Model(carnotaurusData.data)
# Springtrap
springtrapData = Obj("./model/springtrap.obj")
springtrap = Model(springtrapData.data)
# Freddy Fazbear
milesData = Obj("./model/hulkBuster.obj")
miles = Model(milesData.data)
# Iron man
ironmanData = Obj("./model/Mark1.obj")
ironman = Model(ironmanData.data)
# Modelo
modelo = carnotaurus
modelo.position = [0,-5,-10]
modelo.scale = glm.vec3(1,1,1)
modelo.loadTexture("./textures/Carnotaurus.bmp")
rend.scene.append( modelo )

counter_direction = 0

rend.camPosition = glm.vec3([modelo.position[0],modelo.position[1]], 0)
rend.target = modelo.position
rend.camRotation = 0.0

isRunning = True
pygame.mixer.init()
springTrapScream = pygame.mixer.Sound("./sounds/springTrapScream.mp3")
ironmanRepulsor = pygame.mixer.Sound("./sounds/ironmanRepulsor.mp3")
ironmanChest = pygame.mixer.Sound("./sounds/ironmanChest.mp3")
tyranosaurus = pygame.mixer.Sound("./sounds/tyranosaurus.ogg")
jurassicPark = pygame.mixer.Sound("./sounds/JurassicPark.mp3")
belowTheSurface = pygame.mixer.Sound("./sounds/belowTheSurface.mp3")
highWay = pygame.mixer.Sound("./sounds/highway2Hell.mp3")

tyranosaurus.play()
jurassicPark.play(loops=-1)
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

            if event.key == K_m:
                modelo = carnotaurus
                modelo.position = [0,-5,-10]
                modelo.scale = glm.vec3(1,1,1)
                modelo.loadTexture("./textures/Carnotaurus.bmp")
                tyranosaurus.stop()
                ironmanChest.stop()
                ironmanRepulsor.stop()
                springTrapScream.stop()
                jurassicPark.stop()
                belowTheSurface.stop()
                tyranosaurus.play()
                jurassicPark.play(loops=-1)
                

            if event.key == K_n:
                modelo = springtrap
                modelo.position = [0,-10,-10]
                modelo.scale = glm.vec3(0.1,0.1,0.1)
                modelo.loadTexture("./textures/springtrap.png")
                tyranosaurus.stop()
                ironmanChest.stop()
                ironmanRepulsor.stop()
                springTrapScream.stop()
                jurassicPark.stop()
                belowTheSurface.stop()
                springTrapScream.play()
                belowTheSurface.play(loops=-1)
                
            if event.key == K_b:
                modelo = miles
                modelo.position = [0,-5,-10]
                modelo.scale = glm.vec3(2,2,2)
                modelo.loadTexture("./textures/hulkBuster.png")
                tyranosaurus.stop()
                ironmanChest.stop()
                ironmanRepulsor.stop()
                springTrapScream.stop()
                jurassicPark.stop()
                belowTheSurface.stop()
                highWay.stop()
                ironmanChest.play()

            if event.key == K_v:
                modelo = ironman
                modelo.position = [0,-15,-15]
                modelo.scale = glm.vec3(1,1,1)
                modelo.loadTexture("./textures/ironman.png")
                tyranosaurus.stop()
                ironmanChest.stop()
                ironmanRepulsor.stop()
                springTrapScream.stop()
                jurassicPark.stop()
                belowTheSurface.stop()
                highWay.stop()
                ironmanRepulsor.play()
                highWay.play(loops=-1)

    # Direcciones                            
    if keys[K_d] and rend.camPosition.x>-5:
        rend.camPosition.x -= 5 * deltaTime
    
    if keys[K_a] and rend.camPosition.x<5:
        rend.camPosition.x += 5 * deltaTime
    
    if keys[K_w] and rend.camPosition.z<20:
        rend.camPosition.z += 5 * deltaTime
    
    if keys[K_s] and rend.camPosition.z>-5:
        rend.camPosition.z -= 5 * deltaTime
    
    if keys[K_q] and rend.camPosition.y<10:
        rend.camPosition.y += 5 * deltaTime
    
    if keys[K_e] and rend.camPosition.y>-10:
        rend.camPosition.y -= 5 * deltaTime

    # Colores
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
        vert_s = big_shader
        rend.setShaders(vert_s, frag_s)
    if keys[K_8]:
        vert_s = vertex_shader
        rend.setShaders(vert_s, frag_s)
    
    # Fragment shader
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
    
    if keys[K_5]:
        frag_s = fragment_shader
        rend.setShaders(vert_s, frag_s)
    # Reset
    if keys[K_0]: 
        vert_s = vertex_shader
        frag_s = fragment_shader0
        rend.setShaders(vertex_shader, fragment_shader0)

    modelo.rotation.y += 45 * deltaTime     
    rend.scene = [modelo]   
    rend.elapsedTime += deltaTime

    rend.update()
    rend.render()

    pygame.display.flip()

pygame.quit()