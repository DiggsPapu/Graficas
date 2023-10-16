from linealAlgebra import *
from math import tan,pi,atan2,acos

class Intercept(object):
    def __init__(self,distance,point,normal,texcoords,obj):
        self.distance=distance
        self.point=point
        self.normal=normal
        self.texcoords = texcoords
        self.obj=obj
        
        

class Shape(object):
    def __init__(self,position,material):
        self.position = position
        self.material = material

    def ray_intersect(self,orig,dir):
        return None
    
class Sphere(Shape):
    def __init__(self,position,radius,material):
        self.radius = radius
        super().__init__(position,material)
        
    def ray_intersect(self,orig,dir):
        L = subtract(self.position,orig)
        lengthL = norm(L)
        tca = dotProduct(L,dir)
        d = (lengthL**2-tca**2)**0.5
        
        if d>self.radius:
            return None
        
        thc = (self.radius**2-d**2)**0.5
        t0 = tca-thc
        t1 = tca+thc
        
        if t0<0:
            t0=t1
        if t0<0:
            return None
        
        P = add(orig,scalarMultVector(t0,dir))
        normal = subtract(P,self.position)
        normal = normalize(normal)
        
        u = (atan2(normal[2],normal[0])/(2*pi))+0.5
        v = acos(normal[1])/pi

        return Intercept(distance=t0,
                         point=P,
                         normal=normal,
                         texcoords=(u,v),
                         obj=self)
    
class Plane(Shape):
    def __init__(self, position,normal, material):
        self.normal = normalize(normal)
        super().__init__(position, material)
        
    def ray_intersect(self, orig, dir):
        #Distancia (planePos-origRay) producto_punto normal/(dirRay producto_punto normal)
        denom = dotProduct(dir,self.normal)
        
        if abs(denom)<=0.0001:
            return None
        
        num = dotProduct(subtract(self.position,orig),self.normal)
        t = num/denom
        
        if t<0:
            return None
        
        #P=O+D*t0
        P = add(orig,scalarMultVector(t,dir))
        
        return Intercept(distance=t,
                         point=P,
                         normal=self.normal,
                         texcoords=None,
                         obj=self)

class Disk(Plane):
    def __init__(self, position, normal, radius, material):
        self.radius = radius
        super().__init__(position, normal, material)
        
    def ray_intersect(self, orig, dir):
        planeIntersect = super().ray_intersect(orig, dir)
        
        if planeIntersect is None:
            return None
        
        contactDistance = subtract(planeIntersect.point,self.position)
        contactDistance = norm(contactDistance)
        
        if contactDistance > self.radius:
            return None
        
        return Intercept(distance=planeIntersect.distance,
                         point=planeIntersect.point,
                         normal=self.normal,
                         texcoords=None,
                         obj=self)

class AABB(Shape):
    # Axis Aligned Bounding Box
    
    def __init__(self, position, size, material):
        super().__init__(position, material)
        # Listado de planos
        self.planes = []
        # Dimens
        self.size = size
        # Sides
        leftPlane = Plane(add(self.position, [-size[0] / 2,0,0]), (-1,0,0), material=material)
        rightPlane = Plane(add(self.position, [size[0] / 2,0,0]), (1,0,0), material=material)
        bottomPlane = Plane(add(self.position, [0,-size[1] / 2,0]), (0,-1,0), material=material)
        topPlane = Plane(add(self.position, [0,size[1] / 2,0]), (0,1,0), material=material)
        backPlane = Plane(add(self.position, [0,0,-size[2] / 2]), (0,0,-1), material=material)
        frontPlane = Plane(add(self.position, [0,0,size[2] / 2]), (0,0,1), material=material)
        
        self.planes.append(leftPlane)
        self.planes.append(rightPlane)
        self.planes.append(bottomPlane)
        self.planes.append(topPlane)
        self.planes.append(backPlane)
        self.planes.append(frontPlane)
        
        # Bounds, limites
        self.boundsMin = [0,0,0]
        self.boundsMax = [0,0,0]
        # Margen de error
        bias = 0.001
        for i in range(3):
            self.boundsMin[i] = self.position[i] - (bias + size[i]/2)
            self.boundsMax[i] = self.position[i] + (bias + size[i]/2)
        
    def ray_intersect(self, orig, dir):
        intersect = None
        t = float('inf')
        
        u, v = 0, 0    
        
        for plane in self.planes:
            
            planeIntersect = plane.ray_intersect(orig, dir)
            
            if planeIntersect is not None:
                
                planePoint = planeIntersect.point
                
                if self.boundsMin[0] < planePoint[0] < self.boundsMax[0]:
                    if self.boundsMin[1] < planePoint[1] < self.boundsMax[1]:
                        if self.boundsMin[2] < planePoint[2] < self.boundsMax[2]:
                            if planeIntersect.distance < t:
                                t = planeIntersect.distance
                                intersect = planeIntersect
                                
                                # Generar las u's & v's
                                if abs(plane.normal[0]) > 0:
                                    # Estoy en X usamos Y y Z para crear las uvs
                                    u = (planePoint[1] - self.boundsMin[1]) / (self.size[1]+0.002)
                                    v = (planePoint[2] - self.boundsMin[2]) / (self.size[2]+0.002)
                                elif abs(plane.normal[1]) > 0:
                                    # Estoy en Y usamos X y Z para crear las uvs
                                    u = (planePoint[0] - self.boundsMin[0]) / (self.size[0]+0.002)
                                    v = (planePoint[2] - self.boundsMin[2]) / (self.size[2]+0.002)
                                elif abs(plane.normal[2]) > 0:
                                    # Estoy en Z usamos X y Y para crear las uvs
                                    u = (planePoint[0] - self.boundsMin[0]) / (self.size[0]+0.002)
                                    v = (planePoint[1] - self.boundsMin[1]) / (self.size[1]+0.002)
                                        
                                     
                                
        
        if intersect is None:
            return None
        
        return Intercept(distance=t,
                         point=intersect.point,
                         normal=intersect.normal,
                         texcoords=(u, v),
                         obj=self)


class Triangle(object):
    def __init__(self, vertices, material):
        if len(vertices) != 3:
            raise ValueError("A triangle must have exactly three vertices.")
        self.vertices = vertices
        self.material = material

    def ray_intersect(self, orig, dir):
        # extraccion de vertices
        v0, v1, v2 = self.vertices
        # Calcular lados
        e1 = subtract(v1, v0)
        e2 = subtract(v2, v0)
        # Vector ortogonal
        h = crossProduct(dir, e2)
        a = dotProduct(e1, h)
        # Es paralelo al triangulo
        if a > -0.00001 and a < 0.00001:
            return None
        # coordenada de la direccion del rayo
        s = subtract(orig, v0)
        # Calculo de baricentricas
        u = dotProduct(s, h) / a
        q = crossProduct(s, e1)
        v = dotProduct(dir, q) / a
        if u < 0.0 or u > 1.0 or v < 0.0 or u + v > 1.0:
            return None
        # Punto de interseccion
        t = dotProduct(e2, q) / a
        # Si no es paralelo
        if t > 0.00001:
            intersect_point = add(orig, scalarMultVector(t, dir))
            edge1 = subtract(v1, v0)
            edge2 = subtract(v2, v1)
            normal = normalize(crossProduct(edge1, edge2))

            return Intercept(distance=t, point=intersect_point, normal=normal, texcoords=None, obj=self)

        return None

class Cylinder(Shape):
    def __init__(self, position, radius, height, material):
        super().__init__(position, material)
        self.radius = radius
        self.height = height

    def ray_intersect(self, orig, dir):
        # Determinar si el rayo es paralelo al eje y
        if abs(dir[0]) < 0.00001 and abs(dir[2]) < 0.00001:
            if self.position[1] <= orig[1] <= self.position[1] + self.height:
                # En caso de que el rayo sea paralelo y con el mismo rango de altura.
                t_cylinder = ((self.position[0] - orig[0]) ** 2 + (self.position[2] - orig[2]) ** 2 - self.radius ** 2) / (dir[0] ** 2 + dir[2] ** 2)
                y_cylinder = orig[1] + t_cylinder * dir[1]

                if self.position[1] <= y_cylinder <= self.position[1] + self.height:
                    intersect_point = add(orig, scalarMultVector(t_cylinder, dir))
                    normal = subtract(intersect_point, self.position)
                    normal[1] = 0
                    normal = normalize(normal)
                    return Intercept(distance=t_cylinder, point=intersect_point, normal=normal, texcoords=None, obj=self)
        # Aqui se haran los calculos en caso de que se intersecte con el lateral
        val = self.intersectsLateral(orig, dir)
        if val!=None:
            return val
        else:
            # Aquí se haran los calculos en caso de qeu se intersecte con la parte superior o inferior
            return self.intersectsTopBottom(orig, dir)
        
    def intersectsTopBottom(self, orig, dir):
        t_caps = []

        # Interseccion superior
        t_top = (self.position[1] + self.height - orig[1]) / dir[1]
        if self.radius ** 2 >= (orig[0] + t_top * dir[0] - self.position[0]) ** 2 + (orig[2] + t_top * dir[2] - self.position[2]) ** 2:
            t_caps.append(t_top)

        # Interseccion inferior
        t_bottom = (self.position[1] - orig[1]) / dir[1]
        if self.radius ** 2 >= (orig[0] + t_bottom * dir[0] - self.position[0]) ** 2 + (orig[2] + t_bottom * dir[2] - self.position[2]) ** 2:
            t_caps.append(t_bottom)

        if not t_caps:
            return None  # Sin intersecciones

        # Elegir el valor minimo
        t_caps = [t for t in t_caps if t > 0]
        if not t_caps:
            return None  # Las intersecciones estan detras del origen

        t_caps.sort()
        t_cylinder = t_caps[0]

        intersect_point = add(orig, scalarMultVector(t_cylinder, dir))

        # Calcular la normal en el punto de interseccion
        normal = subtract(intersect_point, self.position)
        normal[1] = 0
        normal = normalize(normal)

        return Intercept(distance=t_cylinder, point=intersect_point, normal=normal, texcoords=None, obj=self)

    def intersectsLateral(self, orig, dir):
        t0 = -1
        t1 = -1

        # resolver donde es que se intersecta en t0 y t1
        a = dir[0] * dir[0] + dir[2] * dir[2]
        b = 2 * (dir[0] * (orig[0] - self.position[0]) + dir[2] * (orig[2] - self.position[2]))
        c = (orig[0] - self.position[0]) ** 2 + (orig[2] - self.position[2]) ** 2 - self.radius ** 2

        discriminant = b ** 2 - 4 * a * c

        if discriminant > 0:
            t0 = (-b - (discriminant)**(0.5)) / (2 * a)
            t1 = (-b + (discriminant)**(0.5)) / (2 * a)

        # Chequear si estan en el rango
        if t0 > t1:
            t0, t1 = t1, t0
        # Se utilizara t1 en caso de que t0 sea negativa
        if t0 < 0:
            t0 = t1  # If t0 is negative, use t1
        # No hay interseccion si ambos son negativos
        if t0 < 0:
            return None

        y0 = orig[1] + t0 * dir[1]
        if y0 < self.position[1] or y0 > self.position[1] + self.height:
            return None  # Intersection is outside the height of the cylinder

        intersect_point = add(orig, scalarMultVector(t0, dir))

        # Calcular la normal
        normal = subtract(intersect_point, self.position)
        normal[1] = 0  # Asegurarse que la componente es perpendicular a la superficie
        normal = normalize(normal)

        return Intercept(distance=t0, point=intersect_point, normal=normal, texcoords=None, obj=self)