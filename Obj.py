import re

class Obj(object):
    def __init__(self, filename):
        with open(filename, "r") as file:
            self.lines = file.read().splitlines()
        self.vertices = []
        self.texcoords = []
        self.normals = []
        self.faces = []
        
        for line in self.lines:
            try:
                prefix, value = re.split(r'\s+', line, 1)  # Split based on any amount of whitespace
            except:
                continue
            if prefix == "v":  # Vertices
                self.vertices.append(list(map(float, value.split())))
            elif prefix == "vt":  # Texture Coordinates
                self.texcoords.append(list(map(float, value.split())))
            elif prefix == "vn":  # Normals
                self.normals.append(list(map(float, value.split())))
            elif prefix == "f":  # Faces
                self.faces.append([list(map(int, vert.split("/"))) for vert in value.split()])
