import numpy as np
import math
from numpy import linalg as la
class polygon: # probably just gonna be a triangle
    def __init__(self,points,material):
        self.points = points
        self.normal = []
        self.material = material
    def get_material(self):
        return self.material
    def get_points(self):
        return self.points
    def get_normal(self):
        vec1 = self.points[1] - self.points[0]
        vec2 = self.points[2] - self.points[1]
        normal = np.cross(vec1,vec2)
        no1 = la.norm(normal)
        normal = normal / no1
        self.normal = normal
        # print(normal)
    def get_big_comp(arr):
        pass
    def get_point(self,vec,cam_loc):
        ray_origin = cam_loc
        pn_dot_vec = np.dot(self.normal,vec)
        if pn_dot_vec == 0: # parallel
            return None,-1
        if pn_dot_vec > 0:
            return None,-1 #culled
        dif = [0,0,0] - self.points[0]
        d = np.abs(np.dot(dif,self.normal))
        t = -1 *( np.dot(ray_origin,self.normal)+ d)/pn_dot_vec
        # print(cam_loc,t)
        inter_p = cam_loc + t*vec
        return inter_p ,1
    def intersect(self,vec,cam_loc):
        
        inter_p,good = self.get_point(vec,cam_loc)
        if good == -1:
            return None,None
        n = self.normal
        ba = self.points[1] - self.points[0]
        cb = self.points[2] - self.points[1]  
        ac = self.points[0] - self.points[2]
        qa = inter_p - self.points[0]         
        qb = inter_p - self.points[1]         
        qc = inter_p - self.points[2]
        t1 = np.dot(np.cross(ba,qa),n)
        t2 = np.dot(np.cross(cb,qb),n)         
        t3 = np.dot(np.cross(ac,qc),n)
        # print(t1,t2,t3)
        if t1 < 0 or t2 < 0 or t3 < 0:
            # print("test")
            return None,None        
        # print("good") 
        return inter_p,n    
        
        
        
        '''
        if t <= 0:
            print("failed")
        max_ind = np.argmax(self.normal)
        # print("Before",self.points)
        temp_points = self.points - inter_p
        # print("After",temp_points)
        # print("inter",inter_p)
        temp_points = np.delete(temp_points,[max_ind],axis=1)
        num_cross = 0
        sign = 0
        if temp_points[0][1] <= 0:
            sign = -1
        else:
            sign =1
        for i in range(len(temp_points)):
            # if temp_points[i][0] < 0 and temp_points[i][1] > 0:
            #     print(temp_points[i])
            ip = (i + 1) % len(temp_points)
            # print(i,ip)
            if temp_points[ip][1] <= 0:
                n_sign = -1
            else:
                n_sign = 1
            if sign != n_sign:
                if temp_points[i][0] > 0 and temp_points[ip][0] > 0:
                    num_cross += 1
                elif temp_points[i][0] > 0 or temp_points[ip][0] > 0:
                    uc = (temp_points[i][0] - temp_points[i][1]) * (temp_points[ip][0] - temp_points[i][0]) / (temp_points[ip][1] - temp_points[i][1])
                    if uc > 0:
                        num_cross +=1
            sign = n_sign
        if (num_cross & 1)  == 1:
            # print(inter_p)
            # if inter_p[1] < -5:
            #     return None,None
            return inter_p,self.normal
        return None,None
        '''
