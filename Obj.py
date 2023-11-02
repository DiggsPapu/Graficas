class Obj(object):
    def __init__(self, filename):
        # Se crean los contenedores de los datos del modelo.
        self.data = []
        self.vertices = []
        self.texcoords = []
        self.normals = []
        self.faces = []
        # Asumiendo que el archivo es un formato .obj
        with open(filename,"r") as file:
            self.lines = file.read().splitlines()
        # Por cada linea en el archivo
        for line in self.lines:
            # Si la linea no cuenta con un prefijo y un valor,
            # seguimos a la siguiente linea
            try:
                prefix, value = line.split(" ", 1)
            except:
                continue
            # Dependiendo del prefijo, parseamos y guardamos la informacion
            # en el contenedor correcto
            if prefix == "v": # Vertices
                self.vertices.append( list(map(float, value.split(" "))) )
            elif prefix == "vt": # Texture Coordinates
                self.texcoords.append( list(map(float, value.split(" "))) )
            elif prefix == "vn": # Normals
                self.normals.append( list(map(float, value.split(" "))) )
            elif prefix == "f": # Faces
                self.faces.append([list(map(int, vert.split("/"))) for vert in value.split(" ") ] )
        
        for face in self.faces:
            vertCount = len(face)
            # Vertex 
            v0 = self.vertices[ face[0][0] - 1]
            v1 = self.vertices[ face[1][0] - 1]
            v2 = self.vertices[ face[2][0] - 1]
            if vertCount == 4:
                v3 = self.vertices[ face[3][0] - 1]
            # Texture coords
            vt0 = self.texcoords[face[0][1] - 1]
            vt0 = [vt0[0],vt0[1]]
            vt1 = self.texcoords[face[1][1] - 1]
            vt1 = [vt1[0],vt1[1]]
            vt2 = self.texcoords[face[2][1] - 1]
            vt2 = [vt2[0],vt2[1]]
            if vertCount == 4:
                vt3 = self.texcoords[face[3][1] - 1]
                vt3 = [vt3[0],vt3[1]]
            # Normal coords
            vn0 = self.normals[face[0][2]-1]
            vn1 = self.normals[face[1][2]-1]
            vn2 = self.normals[face[2][2]-1]
            if vertCount == 4:
                vn3 = self.normals[face[3][2]-1]
            # Vertex0
            self.data.extend(v0)
            self.data.extend(vt0)
            self.data.extend(vn0)
            # Vertex1
            self.data.extend(v1)
            self.data.extend(vt1)
            self.data.extend(vn1)
            # Vertex2
            self.data.extend(v2)
            self.data.extend(vt2)
            self.data.extend(vn2)
            if vertCount == 4:
                # Vertex0
                self.data.extend(v0)
                self.data.extend(vt0)
                self.data.extend(vn0)
                # Vertex2
                self.data.extend(v2)
                self.data.extend(vt2)
                self.data.extend(vn2)
                # Vertex3
                self.data.extend(v3)
                self.data.extend(vt3)
                self.data.extend(vn3)