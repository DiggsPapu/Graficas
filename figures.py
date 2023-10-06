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
        v0, v1, v2 = self.vertices
        e1 = subtract(v1, v0)
        e2 = subtract(v2, v0)
        h = crossProduct(dir, e2)
        a = dotProduct(e1, h)
        # Bias
        if a > -0.00001 and a < 0.00001:
            return None
        
        f = 1 / a
        s = subtract(orig, v0)
        u = f * dotProduct(s, h)

        if u < 0.0 or u > 1.0:
            return None

        q = crossProduct(s, e1)
        v = f * dotProduct(dir, q)

        if v < 0.0 or u + v > 1.0:
            return None

        t = f * dotProduct(e2, q)

        if t > 0.00001:
            intersect_point = add(orig, scalarMultVector(t, dir))

            edge1 = subtract(v1, v0)
            edge2 = subtract(v2, v1)
            normal = crossProduct(edge1, edge2)
            normal = normalize(normal)

            return Intercept(distance=t, point=intersect_point, normal=normal, texcoords=None, obj=self)

        return None
    

class Ellipsoid(object):
    def __init__(self, position, radii, material):
        self.position = position
        self.radii = radii
        self.material = material

    def ray_intersect(self, orig, dir):
        # Transform ray into ellipsoid's local coordinate system
        orig = subtract(orig, self.position)
        dir = normalize(dir)

        # Apply scaling factors to the ray
        dir = [dir[0] / self.radii[0], dir[1] / self.radii[1], dir[2] / self.radii[2]]

        # Calculate coefficients of the quadratic equation
        a = dotProduct(dir, dir)
        b = 2 * dotProduct(orig, dir)
        c = dotProduct(orig, orig) - 1  # Assuming the ellipsoid is unit-sized

        # Calculate discriminant
        discriminant = b * b - 4 * a * c

        if discriminant < 0:
            return None  # No intersection

        # Calculate two possible solutions for t (parameter along the ray)
        t1 = (-b - (discriminant)**0.5) / (2 * a)
        t2 = (-b + (discriminant)**0.5) / (2 * a)

        if t1 >= 0 or t2 >= 0:
            # At least one intersection point is in front of the ray's origin
            t = min(t1, t2) if t1 >= 0 and t2 >= 0 else max(t1, t2)

            # Calculate intersection point and normal in local coordinates
            intersection_point_local = add(orig, scalarMultVector(t, dir))
            normal_local = normalize(intersection_point_local)

            # Transform intersection point and normal back to world coordinates
            intersection_point = add(self.position, intersection_point_local)
            normal = scalarMultVector(1.0 / self.radii[0], normal_local)

            return Intercept(distance=t, point=intersection_point, normal=normal, texcoords=None, obj=self)

        return None  # No intersection