OPAQUE = 0
REFLECTIVE = 1
TRANSPARENT = 2

class Material(object):
    def __init__(self,diffuse=(1,1,1),spec=1.0,Ks=0.0, ior = 1.0,texture=None, matType=OPAQUE):
        self.diffuse = diffuse
        self.spec = spec
        self.Ks =Ks
        self.ior = ior
        self.matType = matType
        self.texture = texture