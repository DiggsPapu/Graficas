import pygame
from pygame.locals import *

from rt import RayTracer
from figures import *
from lights import *
from materials import *


width = 256
height = 256

pygame.init()

screen = pygame.display.set_mode((width,height),pygame.DOUBLEBUF|pygame.HWACCEL|pygame.HWSURFACE)
screen.set_alpha(None)

raytracer = RayTracer(screen)
raytracer.envMap = pygame.image.load("./textures/kameHouse.jpeg")
raytracer.rtClearColor(0.25,0.25,0.25)
dragonBall = pygame.image.load("./textures/dragonBall.jpg")
krilinTexture = pygame.image.load("./textures/Krilin.png")
brick = Material(diffuse=(1,0.4,0.4),spec=8,Ks=0.01)
grass = Material(diffuse=(0.4,1,0.4),spec=32,Ks=0.1)
water = Material(diffuse=(0.4,0.4,1),spec=256,Ks=0.2)
mirror = Material(diffuse=(0.9,0.9,0.9),spec=64,Ks=0.2,matType=REFLECTIVE)
blueMirror = Material(diffuse=(0.4,0.4,0.9),spec=32,Ks=0.15,matType=REFLECTIVE)
dragonBall = Material(texture=dragonBall, spec=64, Ks = 0.1, matType=OPAQUE)
krilin = Material(texture=krilinTexture, spec=64, Ks = 0.1, matType=OPAQUE)
glass = Material(diffuse=(0.9,0.9,0.9),spec=64,Ks=0.15,ior=1.5,matType=TRANSPARENT)
diamond = Material(diffuse=(0.9,0.9,0.9),spec=128,Ks=0.2,ior=2.417,matType=TRANSPARENT)
water = Material(diffuse=(0.4,0.4,0.9),spec=128,Ks=0.2,ior=1.33,matType=TRANSPARENT)

raytracer.scene.append(Sphere(position=(-1.7,	+1.0,	-5),		radius=0.8,	material=dragonBall))
raytracer.scene.append(Sphere(position=(-1.7,	-1.5,	-5),		radius=0.8,	material=krilin))
raytracer.scene.append(Sphere(position=(+0.0,	-1.5,	-5),		radius=0.8,	material=mirror))
raytracer.scene.append(Sphere(position=(+0.0,	+1.0,	-5),		radius=0.8,	material=blueMirror))
raytracer.scene.append(Sphere(position=(+1.7,	-1.5,	-5),		radius=0.8,	material=glass))
raytracer.scene.append(Sphere(position=(+1.7,	+1.0,	-5),		radius=0.8,	material=diamond))

raytracer.lights.append(AmbientLight(intensity=0.1))
raytracer.lights.append(DirectionalLight(direction=(-1,-1,-1),intensity=0.9))

raytracer.rtClear()
raytracer.rtRender()

print("\nRender Time:",pygame.time.get_ticks()/1000,"secs")

isRunning = True
while isRunning:
	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			isRunning = False
		elif event.type == pygame.KEYDOWN:
			if event.key == pygame.K_ESCAPE:
				isRunning = False

rect = pygame.Rect(0,0,width,height)
sub = screen.subsurface(rect)
pygame.image.save(sub,"dragonBall.jpg")

pygame.quit()