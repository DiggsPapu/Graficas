from linealAlgebra import * 

from math import acos,asin

def reflectVector(normal,direction):
    reflect = 2*dotProduct(normal,direction)
    reflect = scalarMultVector(reflect,normal)
    reflect = subtract(reflect,direction)
    reflect = normalize(reflect)
    
    return reflect

def refractVector(normal,incident,n1,n2):
    #Snell's Law
    c1 = dotProduct(normal,incident)
    if c1<0:
        c1=-c1
    else:
        normal = negArray(normal)
        n1,n2=n2,n1
    
    n = n1/n2
    
    return normalize(subtract(scalarMultVector(n,(add(incident,scalarMultVector(c1,normal)))),scalarMultVector((1-n**2*(1-c1**2))**0.5,normal)))

def totalInternalReflection(normal,incident,n1,n2):
    
    c1 = dotProduct(normal,incident)
    if c1<0:
        c1=-c1
    else:
        n1,n2=n2,n1
    
    if n1<n2:
        return False
    
    return acos(c1)>=asin(n2/n1)

def fresnel(normal,incident,n1,n2):
    c1 = dotProduct(normal,incident)
    if c1<0:
        c1=-c1
    else:
        n1,n2=n2,n1
     
    s2 = (n1*(1-c1**2)**0.5)/n2
    c2 = (1-s2**2)**0.5
    
    F1 = (((n2*c1)-(n1*c2))/((n2*c1)+(n1*c2)))**2
    F2 = (((n1*c2)-(n2*c1))/((n1*c2)+(n2*c1)))**2
    
    Kr = (F1+F2)/2
    Kt = 1-Kr
    return Kr,Kt

class Light(object):
    def __init__(self,intensity=1,color=(1,1,1),lightType="None"):
        self.intensity = intensity
        self.color = color
        self.lightType = lightType
        
    def getLightColor(self):
        return [self.color[0]*self.intensity,
                self.color[1]*self.intensity,
                self.color[2]*self.intensity]
    
    def getDiffuseColor(self,intercept):
        return None
    
    def getSpecularColor(self,intercept,viewPos):
        return None
    
class AmbientLight(Light):
    def __init__(self,intensity=1,color=(1,1,1)):
        super().__init__(intensity,color,"Ambient")
        
class DirectionalLight(Light):
    def __init__(self, direction=(0,-1,0),intensity=1, color=(1, 1, 1)):
        self.direction=normalize(direction)
        super().__init__(intensity, color,"Directional")
        
    def getDiffuseColor(self,intercept):
        dir = [(i*-1) for i in self.direction]
        
        intensity = dotProduct(intercept.normal,dir)*self.intensity
        intensity = max(0,min(1,intensity))
        intensity *= 1-intercept.obj.material.Ks
        diffuseColor = [(i*intensity) for i in self.color]
        
        return diffuseColor
    
    def getSpecularColor(self, intercept, viewPos):
        dir = [(i*-1) for i in self.direction]
        
        reflect = reflectVector(intercept.normal,dir)
        
        viewDir = subtract(viewPos,intercept.point)
        viewDir = normalize(viewDir)
        
        specIntensity = max(0,dotProduct(viewDir,reflect))**intercept.obj.material.spec
        specIntensity *= intercept.obj.material.Ks
        specIntensity *= self.intensity
        
        return [(i*specIntensity) for i in self.color]
    
class PointLight(Light):
    def __init__(self, point=(0,0,0),intensity=1, color=(1, 1, 1)):
        self.point = point
        super().__init__(intensity, color, "Point")
        
    def getDiffuseColor(self, intercept):
        dir = subtract(self.point,intercept.point)
        R = norm(dir)
        dir = scalarMultVector(1/R,dir)
        
        intensity = dotProduct(intercept.normal,dir)*self.intensity
        intensity *= 1-intercept.obj.material.Ks
        
        #Ley de cuadrados inversos
        # IF = Intensity/R^2
        #R es la distancia del punto intercepto a la luz punto
        if R!=0:
            intensity /= R**2
        
        intensity = max(0,min(1,intensity))
        
        return [(i*intensity) for i in self.color]
    
    def getSpecularColor(self, intercept, viewPos):
        dir = subtract(self.point,intercept.point)
        R = norm(dir)
        dir = scalarMultVector(1/R,dir)
        
        reflect = reflectVector(intercept.normal,dir)
        
        viewDir = subtract(viewPos,intercept.point)
        viewDir = normalize(viewDir)
        
        specIntensity = max(0,dotProduct(viewDir,reflect))**intercept.obj.material.spec
        specIntensity *= intercept.obj.material.Ks
        specIntensity *= self.intensity
        
        if R!=0:
            specIntensity /= R**2
        
        specIntensity = max(0,min(1,specIntensity))
        
        return [(i*specIntensity) for i in self.color]