from math import tan,pi,atan2,acos
from linealAlgebra import *

import pygame
import random

from materials import *
from lights import reflectVector, refractVector, totalInternalReflection, fresnel

MAX_RECURSION_DEPTH = 3

class RayTracer(object):
    def __init__(self,screen):
        self.screen = screen
        _,_,self.width,self.height = screen.get_rect()
        
        self.scene = []
        self.lights = []
        
        self.camPosition = [0,0,0]
        
        self.rtViewport(0,0,self.width,self.height)
        self.rtProyection()
        
        self.rtColor(1,1,1)
        self.rtClearColor(0,0,0)
        self.rtClear()
        
        self.envMap = None
        
    def rtViewport(self,posX,posY,width,height):
        self.vpX = posX
        self.vpY = posY
        self.vpWidth = width
        self.vpHeight = height
        
    def rtProyection(self,fov=60,n=0.1):
        aspectRatio = self.vpWidth/self.vpHeight
        self.nearPlane = n
        self.topEdge = tan((fov*pi/180)/2)*self.nearPlane
        self.rightEdge = self.topEdge*aspectRatio
    
    def rtClearColor(self,r,g,b):
        #Recibe valores de 0 a 1
        self.clearColor = (r*255,g*255,b*255)
       
    def rtClear(self):
        #Pygame usa valores de color de 0 a 255
        self.screen.fill(self.clearColor)

    def rtColor(self,r,g,b):
        self.currColor = (r*255,g*255,b*255)
        
    def rtPoint(self,x,y,color=None):
        y = self.height - y
        if (0<=x<self.width) and (0<=y<self.height):
            if color != None:
                color = (int(color[0]*255),
                         int(color[1]*255),
                         int(color[2]*255))
                self.screen.set_at((x,y),color)
            else:
                self.screen.set_at((x,y),self.currColor)
    
    def rtCastRay(self,orig,dir,sceneObj=None,recursion=0):
        if recursion >= MAX_RECURSION_DEPTH:
            return None
        
        depth = float('inf')
        intercept=None
        hit = None
      

        for obj in self.scene:
            if sceneObj != obj:
                intercept = obj.ray_intersect(orig,dir)
                if intercept!=None:
                    if intercept.distance<depth:
                        hit = intercept
                        depth = intercept.distance
        
        return hit

    def rtRayColor(self,intercept,rayDirection,recursion=0):
        
        if intercept == None:
            if self.envMap:
                x = (atan2(rayDirection[2],rayDirection[0])/(2*pi)+0.5)*self.envMap.get_width()
                y = acos(rayDirection[1])/pi*self.envMap.get_height()
                
                envcolor = self.envMap.get_at((int(x),int(y)))
                return [envcolor[i]/255 for i in range(3)]
            else:
                return None
        
        #Phong Reflection Model
        #LightColor = Ambient + Diffuse + Specular
        #FinalColor = SurfaceColor * LightColor    
        
        material = intercept.obj.material
        surfaceColor = material.diffuse
        
        if material.texture and intercept.texcoords:
            tX= intercept.texcoords[0] * material.texture.get_width() - 1
            tY = intercept.texcoords[1] * material.texture.get_height() - 1
            texcolor = material.texture.get_at((int(tX),int(tY)))
            texcolor = [i/255 for i in texcolor]
            surfaceColor = [surfaceColor[i]*texcolor[i] for i in range(3)]

        reflectColor = [0,0,0]
        refractColor = [0,0,0]
        ambientColor = [0,0,0]
        diffuseColor = [0,0,0]
        specularColor = [0,0,0]
        
        if material.matType == OPAQUE:  
            for light in self.lights:
                if light.lightType=="Ambient":
                    ambientColor = [(ambientColor[i]+light.getLightColor()[i]) for i in range(3)]
                                
                else:
                    lightDir = None
                    if light.lightType=="Directional":
                        lightDir = [(i*-1) for i in light.direction]
                    elif light.lightType=="Point":
                        lightDir = subtract(light.point,intercept.point)
                        lightDir = normalize(lightDir)
                                    
                    shadowIntersect = self.rtCastRay(intercept.point,lightDir,intercept.obj)

                    if shadowIntersect == None:
                        diffuseColor = [(diffuseColor[i]+light.getDiffuseColor(intercept)[i]) for i in range(3)]
                        specularColor = [(specularColor[i]+light.getSpecularColor(intercept,self.camPosition)[i]) for i in range(3)]                                
                        
        elif material.matType == REFLECTIVE:
            reflect = reflectVector(intercept.normal,negArray(rayDirection))
            reflectIntercept = self.rtCastRay(intercept.point,reflect,intercept.obj,recursion+1)
            reflectColor = self.rtRayColor(reflectIntercept,reflect,recursion+1)
            
            for light in self.lights:
                if light.lightType!="Ambient":

                    lightDir = None
                    if light.lightType == "Directional":
                        lightDir = [(i*-1) for i in light.direction]
                    elif light.lightType == "Point":
                        lightDir = subtract(light.point,intercept.point)
                        lightDir = normalize(lightDir)
                        
                    shadowIntersect = self.rtCastRay(intercept.point,lightDir,intercept.obj)

                    if shadowIntersect == None:
                        specularColor = [(specularColor[i]+light.getSpecularColor(intercept,self.camPosition)[i]) for i in range(3)]
        
        elif material.matType == TRANSPARENT:
            outside = dotProduct(rayDirection,intercept.normal)<0
            bias = scalarMultVector(0.001,intercept.normal)

            reflect = reflectVector(intercept.normal,negArray(rayDirection))
            reflectOrig = add(intercept.point,bias) if outside else subtract(intercept.point,bias)
            reflectIntercept = self.rtCastRay(reflectOrig,reflect,None,recursion+1)
            reflectColor = self.rtRayColor(reflectIntercept,reflect,recursion+1)
            
            for light in self.lights:
                if light.lightType!="Ambient":

                    lightDir = None
                    if light.lightType == "Directional":
                        lightDir = [(i*-1) for i in light.direction]
                    elif light.lightType == "Point":
                        lightDir = subtract(light.point,intercept.point)
                        lightDir = normalize(lightDir)
                        
                    shadowIntersect = self.rtCastRay(intercept.point,lightDir,intercept.obj)

                    if shadowIntersect == None:
                        specularColor = [(specularColor[i]+light.getSpecularColor(intercept,self.camPosition)[i]) for i in range(3)]
          
            
            if not totalInternalReflection(intercept.normal,rayDirection,1.0,material.ior):
                refract = refractVector(intercept.normal,rayDirection,1.0,material.ior)
                refractOrig = subtract(intercept.point,bias) if outside else add(intercept.point,bias)
                refractIntercept = self.rtCastRay(refractOrig,refract,None,recursion+1)
                refractColor = self.rtRayColor(refractIntercept,refract,recursion+1)
            
                Kr,Kt = fresnel(intercept.normal,rayDirection,1.0,material.ior)
                reflectColor = scalarMultVector(Kr,reflectColor)
                refractColor = scalarMultVector(Kt,refractColor)
            
        
        lightColor = [(ambientColor[i]+diffuseColor[i]+specularColor[i]+reflectColor[i]+refractColor[i]) for i in range(3)]
        
        return [min(1,surfaceColor[i]*lightColor[i]) for i in range(3)]

    def rtRender(self):
        indeces = [(i,j) for i in range(self.vpWidth) for j in range(self.vpHeight)]
        random.shuffle(indeces)        

        for i, j in indeces:
            x = i + self.vpX
            y = j + self.vpY
            
            if 0<=x<self.width and 0<=y<self.height:
                #Pasar de coordenadas de ventana a 
                #coordenadas NDC (-1 a 1)
                Px = ((x+0.5 - self.vpX)/self.vpWidth)*2-1
                Py = ((y+0.5 - self.vpY)/self.vpHeight)*2-1
                    
                Px *= self.rightEdge
                Py *= self.topEdge
                    
                #Crear un rayo
                direction = (Px,Py,-self.nearPlane)
                direction = normalize(direction)
                    
                intercept = self.rtCastRay(self.camPosition,direction)
                    
                rayColor = self.rtRayColor(intercept,direction)
                    
                if rayColor!=None:
                    self.rtPoint(x,y,rayColor)
                    pygame.display.flip()