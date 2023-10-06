import pygame
from pygame.locals import *

from rt import RayTracer
from figures import *
from lights import *
from materials import *


width = 512
height = 512

pygame.init()

screen = pygame.display.set_mode((width,height),pygame.DOUBLEBUF|pygame.HWACCEL|pygame.HWSURFACE)
screen.set_alpha(None)

raytracer = RayTracer(screen)
raytracer.envMap = pygame.image.load("./textures/kameHouse.jpeg")
raytracer.rtClearColor(0.25,0.25,0.25)
eyeTexture = pygame.image.load("./textures/eye.jpg")
dragonBall = pygame.image.load("./textures/dragonBall.jpg")
krilinTexture = pygame.image.load("./textures/Krilin.png")
brick = Material(diffuse=(1,0.4,0.4),spec=8,Ks=0.01)
red = Material(diffuse=(1,0,0),spec=0,Ks=1)
grass = Material(diffuse=(0.4,1,0.4),spec=32,Ks=0.1)
water = Material(diffuse=(0.4,0.4,1),spec=256,Ks=0.2)
mirror = Material(diffuse=(0.9,0.9,0.9),spec=64,Ks=0.2,matType=REFLECTIVE)
blueMirror = Material(diffuse=(0.4,0.4,0.9),spec=32,Ks=0.15,matType=REFLECTIVE)
eye = Material(texture=eyeTexture, spec=64, Ks = 0.1, matType=OPAQUE)
dragonBall = Material(texture=dragonBall, spec=64, Ks = 0.1, matType=OPAQUE)
krilin = Material(texture=krilinTexture, spec=64, Ks = 0.1, matType=OPAQUE)
glass = Material(diffuse=(0.9,0.9,0.9),spec=64,Ks=0.15,ior=1.5,matType=TRANSPARENT)
diamond = Material(diffuse=(0.9,0.9,0.9),spec=128,Ks=0.2,ior=2.417,matType=TRANSPARENT)
water = Material(diffuse=(0.1,0.4,0.9),spec=128,Ks=0.2,ior=1.33,matType=TRANSPARENT)
# raytracer.scene.append(AABB(position = (-1.5,1.5,-5), size = (1,1,1), material=glass))

# raytracer.scene.append(AABB(position = (-1.0,-1.5,-5), size = (1,1,1), material=krilin))
# raytracer.scene.append(AABB(position = (1.0,1.5,-5), size = (1,1,1), material=mirror))
# raytracer.scene.append(AABB(position = (1.5,-1.5,-5), size = (1,1,1), material=eye))

# raytracer.scene.append(Sphere(position=(-1.7,	+1.0,	-5),		radius=0.8,	material=dragonBall))
# raytracer.scene.append(Sphere(position=(-1.7,	-1.5,	-5),		radius=0.8,	material=krilin))
# raytracer.scene.append(Sphere(position=(+0.0,	-1.5,	-5),		radius=0.8,	material=mirror))
# raytracer.scene.append(Sphere(position=(+0.0,	+1.0,	-5),		radius=0.8,	material=blueMirror))
# raytracer.scene.append(Sphere(position=(+1.7,	-1.5,	-5),		radius=0.8,	material=glass))
# raytracer.scene.append(Sphere(position=(+1.7,	+1.0,	-5),		radius=0.8,	material=diamond))

# raytracer.scene.append(Plane(position=(1,1,10000),normal=(0,-1,0.25),material=glass))
# raytracer.scene.append(Plane(position=(1,1,1),normal=(0,1,0.25),material=blueMirror))
# raytracer.scene.append(Plane(position=(-1,1,1),normal=(1,0,-0.45),material=water))
# raytracer.scene.append(Plane(position=(1,1,1),normal=(1,0,0.45),material=red))
# raytracer.scene.append(Disk(position=(0,-4,-6),normal=(0,1.5,1.5),radius=2,material=mirror))
raytracer.scene.append(Triangle(vertices=[[-1,2,-5],[1,2,-5],[0,0,-5]], material=red))
raytracer.scene.append(Triangle(vertices=[[-1.5,-1,-6],[-1,1,-5],[1,0,-7]], material=blueMirror))
raytracer.scene.append(Triangle(vertices=[[0,-1.5,-4],[1,3,-4],[2,-1.5,-4]], material=glass))
# raytracer.scene.append(Ellipsoid(position=[-1,2,-5], radii=[1,1,1],material=red))
# raytracer.scene.append(Ellipsoid(position=[-1.5,-1,-6], radii=[1,1,1],material=blueMirror))
# raytracer.scene.append(Ellipsoid(position=[0,-1.5,-4], radii=[1,1,1],material=glass))
# raytracer.lights.append(AmbientLight(intensity=0.3))
raytracer.lights.append(DirectionalLight(direction=(-1,-1,-1),intensity=0.9))
# raytracer.lights.append(PointLight(point=(0,-0,-6),intensity=1))

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
pygame.image.save(sub,"rt3.jpg")

pygame.quit()