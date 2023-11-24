import pygame 
from pygame.locals import *
import glm
from Obj import *
from gl import Renderer
from model import Model
from shaders import *
from linearalgebra import *
import glm
from OpenGL.GL import *
from math import pi, sin, cos
width = 960
height = 540

pygame.init()

screen = pygame.display.set_mode((width, height), pygame.OPENGL | pygame.DOUBLEBUF)
clock = pygame.time.Clock()

rend = Renderer(screen)
o = [[1,0,0],[-1,0,0], [0,1,0],[0,0,1],[0,0,-1]]
# Crear skyboxes
dinosaurAmbient = ["./textures/skybox2/px.png",
                  "./textures/skybox2/nx.png",
                  "./textures/skybox2/py.png",
                  "./textures/skybox2/ny.png",
                  "./textures/skybox2/pz.png",
                  "./textures/skybox2/nz.png"]
caveAmbient = [
                    "./textures/skybox3/px.png",
                    "./textures/skybox3/nx.png",
                    "./textures/skybox3/py.png",
                    "./textures/skybox3/ny.png",
                    "./textures/skybox3/pz.png",
                    "./textures/skybox3/nz.png"]    
freddysAmbient = [
                    "./textures/skybox4/px.png",
                    "./textures/skybox4/nx.png",
                    "./textures/skybox4/py.png",
                    "./textures/skybox4/ny.png",
                    "./textures/skybox4/pz.png",
                    "./textures/skybox4/nz.png"]
infinityWarAmbient = [
                    "./textures/skybox5/px.png",
                    "./textures/skybox5/nx.png",
                    "./textures/skybox5/py.png",
                    "./textures/skybox5/ny.png",
                    "./textures/skybox5/pz.png",
                    "./textures/skybox5/nz.png"]
    
skyboxes = [dinosaurAmbient, 
            freddysAmbient, 
            infinityWarAmbient, 
            # caveAmbient
            ]
for x in skyboxes:
    rend.loadSkyBox(x)
skyboxindex = 0
rend.createSkybox(dinosaurAmbient, skybox_vertex_shader, skybox_fragment_shader)
rend.renderSkyBox(skyboxindex)

vert_s = vertex_shader
frag_s = fragment_shader0
rend.setShaders(vertex_shader, fragment_shader0)

# Carnotaurus
carnotaurusData = Obj("./model/Carnotaurus.obj")
carnotaurus = Model(carnotaurusData.data)
carnotaurus.position = [0,0,-10]
carnotaurus.scale = glm.vec3(1,1,1)
carnotaurus.loadTexture("./textures/Carnotaurus.bmp")
# Springtrap
springtrapData = Obj("./model/springtrap.obj")
springtrap = Model(springtrapData.data)
springtrap.position = [0,0,-10]
springtrap.scale = glm.vec3(0.1,0.1,0.1)
springtrap.loadTexture("./textures/springtrap.png")
# Freddy Fazbear
hulkbusterData = Obj("./model/hulkBuster.obj")
hulkbuster = Model(hulkbusterData.data)
hulkbuster.position = [0,0,-10]
hulkbuster.scale = glm.vec3(2,2,2)
hulkbuster.loadTexture("./textures/hulkBuster.png")
# Iron man
ironmanData = Obj("./model/Mark1.obj")
ironman = Model(ironmanData.data)
ironman.position = [0,0,-15]
ironman.scale = glm.vec3(1,1,1)
ironman.loadTexture("./textures/ironman.png")
# Modelo
modelo = carnotaurus
rend.scene.append( modelo )

counter_direction = 0

rend.target = modelo.position

isRunning = True
# SOUNDS
pygame.mixer.init()
# Sounds
springTrapScream = pygame.mixer.Sound("./sounds/springTrapScream.mp3")
ironmanRepulsor = pygame.mixer.Sound("./sounds/ironmanRepulsor.mp3")
ironmanChest = pygame.mixer.Sound("./sounds/ironmanChest.mp3")
tyranosaurus = pygame.mixer.Sound("./sounds/tyranosaurus.ogg")
# Music
jurassicPark = pygame.mixer.Sound("./sounds/JurassicPark.mp3")
belowTheSurface = pygame.mixer.Sound("./sounds/belowTheSurface.mp3")
highWay = pygame.mixer.Sound("./sounds/highway2Hell.mp3")
avengers = pygame.mixer.Sound("./sounds/Avengers.mp3")

tyranosaurus.play()
jurassicPark.play(loops=-1)

while isRunning:
    deltaTime = clock.tick(60) / 1000
    keys = pygame.key.get_pressed()
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            isRunning = False
        # Mouse wheel
        elif event.type == pygame.MOUSEWHEEL:
            if event.y == -1 and rend.camPosition.z <10:
                rend.camPosition.z += 50 * deltaTime
            elif event.y == 1 and rend.camPosition.z > -10:
                rend.camPosition.z -= 50 * deltaTime
        # Cambio de perspectiva del modelo
        elif event.type == pygame.MOUSEMOTION and event.buttons[0] == 1:
            rel = pygame.mouse.get_rel()
            sensitivity = 0.2 
            modelo.rotation.x += rel[1] * sensitivity
            modelo.rotation.y += rel[0] * sensitivity    
        # le da direccion en x para rotar con el boton del scroll
        elif event.type == pygame.MOUSEMOTION and event.buttons[2]==1:
            rel = pygame.mouse.get_rel()
            # Para que no haya division por 0
            if rel[0]!=0:
                rend.camAngle +=rel[0]/abs(rel[0])
                if rend.camAngle == 0.0:
                    rend.camAngle = 360
                cambio = 5*deltaTime
                # Parametricas, no funcionaron
                # if rend.camPosition.x+ cambio < abs(modelo.position[2]):
                #     rend.camPosition.x = (abs(modelo.position[2])**2-(rend.camPosition.z-modelo.position[2])**2)**(0.5)- cambio
                    # rend.camPosition.z = (abs(modelo.position[2])**2-(rend.camPosition.x-modelo.position[0])**2)**(0.5)- cambio
                # Polares
                rend.camPosition.x = rend.target[0] + abs(modelo.position[2]) * sin(rend.camAngle * 2 * pi / 360)
                rend.camPosition.z = rend.target[2] + abs(modelo.position[2]) * cos(rend.camAngle * 2 * pi / 360)
        elif event.type == pygame.MOUSEMOTION and event.buttons[1]==1:
            rel = pygame.mouse.get_rel()
            if rel[1] != 0 and rend.camPosition.y>-10 and rend.camPosition.y<10:
                rend.camPosition.y += 5 * deltaTime * rel[1]/abs(rel[1])
        elif event.type == pygame.KEYDOWN:
            # Exit
            if event.key == K_ESCAPE:
                isRunning = False
            # Just triangles and without skybox
            if event.key == pygame.K_SPACE:
                rend.toggleFilledMode()
            # Direction of light
            if event.key == K_l:
                counter_direction+=1
                dirl = o[counter_direction%5]
                rend.dirLight = glm.vec3(dirl[0], dirl[1], dirl[2])
            # CHANGE MODELS
            if event.key == K_m:
                modelo = carnotaurus
                tyranosaurus.stop()
                ironmanChest.stop()
                ironmanRepulsor.stop()
                springTrapScream.stop()
                jurassicPark.stop()
                belowTheSurface.stop()
                avengers.stop()
                highWay.stop()
                tyranosaurus.play()
                jurassicPark.play(loops=-1)

            if event.key == K_n:
                modelo = springtrap
                tyranosaurus.stop()
                ironmanChest.stop()
                ironmanRepulsor.stop()
                springTrapScream.stop()
                jurassicPark.stop()
                belowTheSurface.stop()
                avengers.stop()
                highWay.stop()
                springTrapScream.play()
                belowTheSurface.play(loops=-1)
                
            if event.key == K_b:
                modelo = hulkbuster
                tyranosaurus.stop()
                ironmanChest.stop()
                ironmanRepulsor.stop()
                springTrapScream.stop()
                jurassicPark.stop()
                belowTheSurface.stop()
                highWay.stop()
                avengers.stop()
                ironmanChest.play()
                avengers.play(loops=-1)

            if event.key == K_v:
                modelo = ironman
                tyranosaurus.stop()
                ironmanChest.stop()
                ironmanRepulsor.stop()
                springTrapScream.stop()
                jurassicPark.stop()
                belowTheSurface.stop()
                highWay.stop()
                avengers.stop()
                ironmanRepulsor.play()
                highWay.play(loops=-1)
            
            if event.key == K_c:
                skyboxindex+=1
                rend.renderSkyBox(skyboxindex)

    # Directions of the camera                            
    if keys[K_a] :
        rend.camAngle -=1
        if rend.camAngle == 0.0:
            rend.camAngle = 360
        cambio = 5*deltaTime
        # Parametricas, no funcionaron
        # if rend.camPosition.x+ cambio < abs(modelo.position[2]):
        #     rend.camPosition.x = (abs(modelo.position[2])**2-(rend.camPosition.z-modelo.position[2])**2)**(0.5)- cambio
            # rend.camPosition.z = (abs(modelo.position[2])**2-(rend.camPosition.x-modelo.position[0])**2)**(0.5)- cambio
        # Polares
        rend.camPosition.x = rend.target[0] + abs(modelo.position[2]) * sin(rend.camAngle * 2 * pi / 360)
        rend.camPosition.z = rend.target[2] + abs(modelo.position[2]) * cos(rend.camAngle * 2 * pi / 360)
    
    if keys[K_d] :
        rend.camAngle +=1
        if rend.camAngle == 360:
            rend.camAngle = 0.0
        rend.camPosition.x = rend.target[0] + abs(modelo.position[2]) * sin(rend.camAngle * 2 * pi / 360)
        rend.camPosition.z = rend.target[2] + abs(modelo.position[2]) * cos(rend.camAngle * 2 * pi / 360)
    
    if keys[K_s]:
        if rend.camPosition.z <10:
            rend.camPosition.z += 5 * deltaTime
    
    if keys[K_w]:
        if rend.camPosition.z >-10:
            rend.camPosition.z -= 5 * deltaTime
    
    if keys[K_e] and rend.camPosition.y < 10:
        if rend.camPosition.y <10:
            rend.camPosition.y += 5 * deltaTime
    
    if keys[K_q]:
        if rend.camPosition.y >-10:
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
    rend.render(skyboxindex)

    pygame.display.flip()

pygame.quit()