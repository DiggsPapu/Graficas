from linealAlgebra import *
def reflectVector(normal,direction):
    reflect = 2*dotProduct(normal,direction)
    reflect = scalarMultVector(reflect,normal)
    reflect = subtract(reflect,direction)
    reflect = normalize(reflect)
    return reflect
class Light(object):
    def __init__(self,intensity = 1, color = (1,1,1), lightType = "None"):
        self.intensity = intensity
        self.color = color
        self.type = type
        self.lightType = lightType
    def getLightColor(self):
        return [self.color[0] * self.intensity,
                self.color[1] * self.intensity,
                self.color[2] * self.intensity]
    def getDiffuseColor(self,intercept):
        return None
    def getSpecular(self,intercept,viewPos):
        return None
    
class AmbientLight(Light):
    def __init__(self, intensity=1, color=(1, 1, 1)):
        super().__init__(intensity, color, "Ambient")
class DirectionalLight(Light):
    def __init__(self, direction = (0,-1,0), intensity = 1, color = (1,1,1)):
        self.direction = normalize(direction)
        super().__init__(intensity,color, "Directional")
    def getDiffuseColor(self,intercept):
        dir = [(i*-1) for i in self.direction]
        intensity = dotProduct(intercept.normal, dir)*self.intensity
        intensity = max(0,min(1,intensity))
        intensity *= 1 - intercept.obj.material.Ks
        return [(i*intensity) for i in self.color]
    def getSpecularColor(self,intercept,viewPos):
        dir = [(i*-1) for i in self.direction]
        reflect = reflectVector(intercept.normal,dir)
        viewDir = subtract(viewPos,intercept.point)
        viewDir = normalize(viewDir)
        specIntensity = max(0,dotProduct(viewDir,reflect))**intercept.obj.material.spec
        specIntensity *= intercept.obj.material.Ks
        specIntensity *= self.intensity
        specColor = [(i*specIntensity) for i in self.color]
        return specColor

class PointLight(Light):
    def __init__(self, point = (0,0,0), intensity=1, color=(1, 1, 1)):
        super().__init__(intensity, color, "Point")
        self.point = point
    def getDiffuseColor(self, intercept):
        dir = subtract(self.point,intercept.point)
        R = norm(dir)
        dir = scalarMultVector(1/R,dir)
        intensity = dotProduct(intercept.normal, dir)*self.intensity
        intensity *= 1 - intercept.obj.material.Ks
        # Ley de cuadrados inversos
        # FinalIntensity = Intensity /R^2
        # R es la distancia del punto intercepto a la luz punto
        if R != 0:    
            intensity /= R**2
        intensity = max(0,min(1,intensity))
        return [(i*intensity) for i in self.color]
    def getSpecularColor(self,intercept,viewPos):
        dir = subtract(self.point,intercept.point)
        R = norm(dir)
        dir = scalarMultVector(1/R,dir)
        reflect = reflectVector(intercept.normal,dir)
        viewDir = subtract(viewPos,intercept.point)
        viewDir = normalize(viewDir)
        specIntensity = max(0,dotProduct(viewDir,reflect))**intercept.obj.material.spec
        specIntensity *= intercept.obj.material.Ks
        specIntensity *= self.intensity
        if R != 0:
            specIntensity /= R**2
        specIntensity = max(0,min(1,specIntensity))
        return [(i*specIntensity) for i in self.color]