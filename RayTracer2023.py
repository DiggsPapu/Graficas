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

mercury = Material(texture=pygame.image.load("./textures/mercuryTexture.jpg"),spec=0.1,Ks=0.5,ior=0.0,matType=REFLECTIVE)
raytracer.scene.append(Sphere(position=(1.25,	0.1,	-6),		radius=0.65,	material=mercury))

venus = Material(texture=pygame.image.load("./textures/venusTexture.png"), matType=OPAQUE)
raytracer.scene.append(Sphere(position=(0.6,	-0.8,	-3),		radius=0.75,	material=venus))

earth = Material(texture=pygame.image.load("./textures/earthTexture.jpg"),  spec=0.1,Ks=0.3,ior=5.0,matType=OPAQUE)
raytracer.scene.append(Sphere(position=(-0.5,	-0.6,	-2),		radius=0.75,	material=earth))

mars = Material(texture=pygame.image.load("./textures/marsTexture.jpg"), matType=OPAQUE)
raytracer.scene.append(Sphere(position=(+2.3,	-0.7,	-5),		radius=0.8,	material=mars))

moon = Material(texture=pygame.image.load("./textures/moonTexture.jpeg"), matType=OPAQUE)
raytracer.scene.append(Sphere(position=(-0.4,	-0.2,	-1),		radius=0.15,	material=moon))

jupyter = Material(texture=pygame.image.load("./textures/jupyterTexture.jpg"), matType=OPAQUE)
raytracer.scene.append(Sphere(position=(0.2,	0.8,	-7),		radius=0.4,	material=jupyter))

sun = Material(texture=pygame.image.load("./textures/SunTexture.jpg"), spec=0.1,Ks=1.0,ior=7.0, matType=REFLECTIVE)
raytracer.scene.append(Sphere(position=(-2.0,	3.0,	-15),		radius=1.0,	material=sun))

pinkMirror = Material(diffuse=(0.9,0.4,0.4),spec=32,Ks=0.15,matType=REFLECTIVE)
val = Model('./model/astronaut.obj',material=pinkMirror,scale=(0.1,0.1,0.1),translate=(-0.2,-0.7,-1.0))
raytracer.scene.extend(val.primitives)

# UFO
glass2 = Material(diffuse=(0.5, 1.0, 0.5),spec=1000,Ks=0.1,ior=1000,matType=TRANSPARENT)
raytracer.scene.append(Triangle(vertices=[[-5,2,-10],[-4,4,-10],[-3,2,-10]], material=glass2))
glass3 = Material(diffuse=(0.5, 0.5, 1.0),spec=64,Ks=0.1,ior=1.2,matType=TRANSPARENT)
raytracer.scene.append(Triangle(vertices=[[-5,2,-10],[-4,0,-10],[-3,2,-10]], material=glass3))
glassWeird = Material(diffuse=(1.0,0.5,0.5),spec=256,Ks=0.4,ior=2.57,matType=TRANSPARENT)
raytracer.scene.append(Cylinder(position=(-4,1.7,-10), radius=1,height=0.5,material=glassWeird))

raytracer.lights.append(AmbientLight(intensity=0.3))
raytracer.lights.append(DirectionalLight(direction=(0,-1,0),intensity=0.8))
raytracer.lights.append(DirectionalLight(direction=(0,0,1),intensity=0.4))
# Mercury
raytracer.lights.append(PointLight(point=(1.25,	0.1,	-6), intensity=1, color=(1,1,1)))
# Sun Light
raytracer.lights.append(PointLight(point=(-2.0,	3.0,	-15), intensity=1, color=(1,1,1)))
raytracer.lights.append(PointLight(point=(-1.9,	2.4,	-14), intensity=1, color=(1,1,1)))
raytracer.lights.append(PointLight(point=(-1.7,	2.4,	-14), intensity=1, color=(1,1,1)))
raytracer.lights.append(PointLight(point=(-1.5,	2.4,	-14), intensity=1, color=(1,1,1)))
raytracer.lights.append(PointLight(point=(-2.1,	2.4,	-14), intensity=1, color=(1,1,1)))
raytracer.lights.append(PointLight(point=(-2.3,	2.4,	-14), intensity=1, color=(1,1,1)))
raytracer.lights.append(PointLight(point=(-1.3,	2.4,	-14), intensity=1, color=(1,1,1)))
raytracer.lights.append(PointLight(point=(-1.9,	2.6,	-14), intensity=1, color=(1,1,1)))
raytracer.lights.append(PointLight(point=(-1.7,	2.6,	-14), intensity=1, color=(1,1,1)))
raytracer.lights.append(PointLight(point=(-1.5,	2.6,	-14), intensity=1, color=(1,1,1)))
raytracer.lights.append(PointLight(point=(-2.1,	2.6,	-14), intensity=1, color=(1,1,1)))
raytracer.lights.append(PointLight(point=(-2.3,	2.6,	-14), intensity=1, color=(1,1,1)))
raytracer.lights.append(PointLight(point=(-1.3,	2.6,	-14), intensity=1, color=(1,1,1)))
# Astronaut
raytracer.lights.append(PointLight(point=(-0.2,-0.7,-0.0), intensity=1, color=(1,1,1)))
raytracer.lights.append(PointLight(point=(-0.25,-0.7,-0.0), intensity=1, color=(1,1,1)))
raytracer.lights.append(PointLight(point=(-0.15,-0.7,-0.0), intensity=1, color=(1,1,1)))
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
pygame.image.save(sub,"project2_1.jpg")

pygame.quit()