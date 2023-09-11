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
brick = Material(diffuse=(1,0.4,0.4), spec = 8 , Ks = 0.01)
grass = Material(diffuse=(0.4,1,0.4), spec = 32 , Ks = 0.1)
water = Material(diffuse=(0.4,0.4,1), spec = 256, Ks = 0.2)
# raytracer.scene.append( Sphere(position = (-2,0,-5), radius= 0.5, material = brick))
# raytracer.scene.append( Sphere(position = (0,0,-3), radius= 0.5, material = grass))
# raytracer.scene.append( Sphere(position = (2,0,-5), radius= 0.5, material = water))
raytracer.scene.append( Sphere(position = (1,1,-5), radius = 0.5, material = grass))
raytracer.scene.append( Sphere(position = (0,0,-7), radius = 2, material = brick))
raytracer.scene.append( Sphere(position = (0.5,-1,-5), radius = 0.5, material = water))
raytracer.lights.append( AmbientLight(intensity = 0.1) )
# raytracer.lights.append( DirectionalLight(direction = (-1,-1,-1),intensity = 0.7) )
# raytracer.lights.append( DirectionalLight(direction = (0,1,-1),intensity = 0.5,color=(1,0,1)) )
raytracer.lights.append(PointLight(point = (2.5,0,-5), intensity = 1,color = (1,0,1)))
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