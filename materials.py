class Material(object):
    def __init__(self, diffuse = (1,1,1), spec = 1.0, Ks = 0.0):
        self.diffuse = diffuse
        self.spec = spec
        self.Ks = Ks