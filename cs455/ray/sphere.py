import numpy as np
import math
from numpy import linalg as la
class sphere:
    def __init__(self,x,y,z,r,material):
        self.loc = np.asarray([x,y,z])
        self.radius = r
        self.r2 =r*r
        self.material = material
    def get_material(self):
        return self.material
    def get_color(self):
        return self.color
    def normalize(self,ray):
        ray2 = np.dot(ray,ray)
        return ray/math.sqrt(ray2)
    def intersect(self,ray_direction,ray_origin):
        # print(ray_direction)
        # ray_d = ray_direction / la.norm(ray_direction)
        ray_d = ray_direction
        oc = self.loc - ray_origin
        dis = la.norm(oc)
        
        tca = np.dot(oc,ray_d)
        if tca < 0:
            return None,None
        d2 = np.dot(oc,oc) - tca * tca
        # d = math.sqrt(d2)
        if d2 > self.r2:
            return None,None
        thc2 = self.r2 - d2
        if dis < self.radius:
            t = tca + np.sqrt(thc2)
        else:
            t = tca - np.sqrt(thc2)
        inter = ray_origin + ray_d * t
        normal = inter - self.loc
        
        return inter,self.normalize(normal)