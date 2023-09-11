import pygame
from pygame.locals import *
from rt import Raytracer
from figures import *
from lights import *
from materials import *
width = 512
height = 512
pygame.init()
screen = pygame.display.set_mode((width, height), pygame.DOUBLEBUF | pygame.HWACCEL | pygame.HWSURFACE)
screen.set_alpha(None)
isRunning = True
raytracer = Raytracer(screen)
raytracer.rtClearColor(0.25,0.25,0.25)

nose = Material(diffuse=(1,0.4,0.1), spec = 0, Ks = 0.8)
snowBalls = Material(diffuse=(1,0.97,0.97), spec = 0, Ks = 0.9)
whiteEye = Material(diffuse=(1,1,1),spec=1000,Ks=0)
pupils = Material(diffuse=(0.5,0.5,0.5),spec=1,Ks=1)
mouth = Material(diffuse=(0.3,0.3,0.3),spec=0,Ks=0.9)
button = Material(diffuse=(0.01,0.1,0.1),spec=0,Ks=0)
# Nose
raytracer.scene.append(Sphere(position=(-0.75,4.2,-10), radius=0.35,material=nose))
# Eyes
raytracer.scene.append(Sphere(position=(-0.5,4.3,-9), radius=0.15,material=whiteEye))
raytracer.scene.append(Sphere(position=(-1.2,4.4,-9), radius=0.15,material=whiteEye))
# Pupils
raytracer.scene.append(Sphere(position=(-0.5+0.05,4.3-0.45,-8), radius=0.09,material=pupils))
raytracer.scene.append(Sphere(position=(-1.2+0.13,4.4-0.46,-8), radius=0.09,material=pupils))
# Mouth
raytracer.scene.append(Sphere(position=(-1.2,3.4-0.5,-8), radius=0.09, material = mouth))
raytracer.scene.append(Sphere(position=(-0.9,3.3-0.5,-8), radius=0.09, material = mouth))
raytracer.scene.append(Sphere(position=(-0.5,3.3-0.5,-8), radius=0.09, material = mouth))
raytracer.scene.append(Sphere(position=(-0.2 ,3.4-0.5,-8), radius=0.09, material = mouth))
# Body
raytracer.scene.append(Sphere(position=(-1,5,-12), radius=1.5,material=snowBalls))
raytracer.scene.append(Sphere(position=(-1.2,2.2,-15), radius=2.7,material=snowBalls))
raytracer.scene.append(Sphere(position=(-0.95,-2,-12), radius=2.5,material=snowBalls))
# Buttons
raytracer.scene.append(Sphere(position=(-0.75, 1.7,-9), radius=0.35,material=button))
raytracer.scene.append(Sphere(position=(-0.75, 0.3,-9), radius=0.35,material=button))
raytracer.scene.append(Sphere(position=(-0.75,-1.4,-9), radius=0.55,material=button))
# raytracer.scene.append( Sphere(position = (-2,0,-5), radius= 0.5, material = brick))
# raytracer.scene.append( Sphere(position = (0,0,-3), radius= 0.5, material = grass))
# raytracer.scene.append( Sphere(position = (2,0,-5), radius= 0.5, material = water))
# raytracer.scene.append( Sphere(position = (1,1,-5), radius = 0.5, material = grass))
# raytracer.scene.append( Sphere(position = (0,0,-7), radius = 2, material = brick))
# raytracer.scene.append( Sphere(position = (0.5,-1,-5), radius = 0.5, material = water))
raytracer.lights.append( AmbientLight(intensity = 0.15) )
raytracer.lights.append( DirectionalLight(direction = (-1,5,-20),intensity = 0.7) )
# raytracer.lights.append( DirectionalLight(direction = (0,1,-1),intensity = 0.5,color=(1,0,1)) )
raytracer.lights.append(PointLight(point = (-0.5+0.05,4.3-0.45,-8), intensity = 0.001,color = (1,1,1)))
# raytracer.lights.append(PointLight(point = (-1.2,4.4,-9), intensity = 0.4,color = (1,1,1)))
while isRunning:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            isRunning = False
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                isRunning = False 
    raytracer.rtClear()
    raytracer.rtRender()   
    pygame.display.flip()
pygame.quit()