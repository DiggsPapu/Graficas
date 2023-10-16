import pygame
from pygame.locals import *
from obj import *
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
raytracer.envMap = pygame.image.load("./textures/galaxy.jpg")
raytracer.rtClearColor(0.25,0.25,0.25)

# It'll represent a black hole bc the sun exploded
glass2 = Material(diffuse=(0.5, 1.0, 0.5),spec=64,Ks=0.1,ior=1.2,matType=TRANSPARENT)
raytracer.scene.append(Triangle(vertices=[[-3,-2,-10],[0,6,-10],[3,-2,-10]], material=glass2))
glass3 = Material(diffuse=(0.5, 0.5, 0.9),spec=64,Ks=0.1,ior=1.2,matType=TRANSPARENT)
raytracer.scene.append(Triangle(vertices=[[-4,3.5,-10],[0,-6,-10],[4,3.5,-10]], material=glass3))

mercury = Material(texture=pygame.image.load("./textures/mercuryTexture.jpg"), matType=OPAQUE)
raytracer.scene.append(Sphere(position=(+2.3,	2.0,	-9),		radius=0.65,	material=mercury))

venus = Material(texture=pygame.image.load("./textures/venusTexture.png"), matType=OPAQUE)
raytracer.scene.append(Sphere(position=(+2.3,	1.0,	-8),		radius=0.65,	material=venus))

earth = Material(texture=pygame.image.load("./textures/earthTexture.jpg"), matType=OPAQUE)
raytracer.scene.append(Sphere(position=(+2.3,	0.0,	-7),		radius=0.7,	material=earth))

moon = Material(texture=pygame.image.load("./textures/moonTexture.jpeg"), matType=OPAQUE)
raytracer.scene.append(Sphere(position=(+3.0,	0.5,	-6.5),		radius=0.3,	material=moon))


mars = Material(texture=pygame.image.load("./textures/marsTexture.jpg"), matType=OPAQUE)
raytracer.scene.append(Sphere(position=(+2.3,	-1.0,	-6),		radius=0.7,	material=mars))

jupyter = Material(texture=pygame.image.load("./textures/jupyterTexture.jpg"), matType=OPAQUE)
raytracer.scene.append(Sphere(position=(+2.3,	-2.0,	-5),		radius=0.8,	material=jupyter))

glassWeird = Material(diffuse=(1,1,1),spec=256,Ks=0.4,ior=2.57,matType=TRANSPARENT)
raytracer.scene.append(Cylinder(position=[-1.0,	-1.25,	-5], radius=1.5,height=0.5,material=glassWeird))


ovni = Material(diffuse=(0.8,0.67,0.3),spec=256,Ks=0.4,ior=2.57,matType=REFLECTIVE)
raytracer.scene.append(Cylinder(position=[3.0,	2.3,	-5], radius=1.5,height=0.5,material=ovni))

# pinkMirror = Material(diffuse=(0.9,0.4,0.4),spec=32,Ks=0.15,matType=REFLECTIVE)
# val = Model('./model/astronaut.obj',material=pinkMirror,scale=(1,1,1),translate=(-1.7,-2.0,-4.0))
# raytracer.scene.extend(val.primitives)

raytracer.lights.append(AmbientLight(intensity=0.3))
raytracer.lights.append(DirectionalLight(direction=(-1,-1,-1),intensity=0.9))
raytracer.lights.append(PointLight(point=(-1.7,-2.0,-4.0),intensity=1))

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
pygame.image.save(sub,"project2.jpg")

pygame.quit()