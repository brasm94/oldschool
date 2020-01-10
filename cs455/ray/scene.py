import numpy as np
from sphere import sphere
from polygon import polygon
import math
class scene:
    def __init__(self,x_size,y_size):
        self.dim = [] # -x,x -y,y -z,z
        self.objects = []
        self.camera = []   #LookFrom and LookAt
        self.light = [] #lookFrom and LookAt
        self.window = {}
        self.view_port_width = x_size
        self.view_port_height = y_size
        self.light_color = np.asarray([1,1,1])
        self.ambient = np.asarray([.1,.1,.1])
        self.background = np.asarray([.2,.2,.2])
    def add_sphere(self,x,y,z,r,col):
        self.objects.append(sphere(x,y,z,r,col))
    def add_poly(self,points,mat):
        poly = polygon(np.asarray(points),mat)
        poly.get_normal()
        self.objects.append(poly)
        # print(self.objects[0].get_points())
    def add_light(self,l):
        self.light = l
        self.light_color = [1,1,1]
    def set_camera(self,lf_x,lf_y,lf_z,la_x,la_y,la_z):
        self.camera = np.asarray([lf_x,lf_y,lf_z,la_x,la_y,la_z]) # lf = LookFrom la = Lookat
    def set_window(self,z,x_min,y_min,x_max,y_max):
        self.window['z'] = z
        self.window['x_min'] = x_min
        self.window['y_min'] = y_min
        self.window['x_max'] = x_max
        self.window['y_max'] = y_max
    def pixel2point(self):
        if len(self.window) == 0:
            print("window not initialized")
            raise ValueError("window not initialized line 20 RN")
        row_scaled = (self.window['y_max'] - self.window['y_min']) / self.view_port_height
        col_scaled = (self.window['x_max'] - self.window['x_min']) / self.view_port_width
        pixel_loc = []
        zt = self.window['z']
        for t_row in reversed(range(self.view_port_height)):
            for t_col in range(self.view_port_width):
                rt = t_row * row_scaled + self.window['y_min']
                ct = t_col * col_scaled + self.window['x_min']
                pixel_loc.append([ct,rt,zt])
        return pixel_loc
    def normalize(self,ray):
        ray2 = np.dot(ray,ray)
        return ray/math.sqrt(ray2)
    def point2cam(self,point):
        temp = point - self.get_cam_loc()
        return math.sqrt(np.dot(temp,temp))
    def get_cam_loc(self):
        return self.camera[0:3]
    def get_inter_objects(self,vec):
        ob_inter = []
        for i,obj in enumerate(self.objects):
            o,n = obj.intersect(vec,self.get_cam_loc())
            if o is not None:
                ob_inter.append([o,i,n])
        return ob_inter
    def get_close_object(self,objs):
        best = np.inf
        best_i = -1
        best_point = []
        best_n = np.asarray([0,0,0])
        for th in objs:
            if self.point2cam(th[0]) < best:
                best_i = th[1]
                best_point = th[0]
                best_n = th[2]
        return best_i,best_point,best_n
    def get_intersect(self,vec):
        ob_inter = self.get_inter_objects(vec)
        if len(ob_inter) == 0:
            return self.background
        best_i,best_point,best_n = self.get_close_object(ob_inter)

        shadow_ray = self.light - best_point
        shadow_ray = self.normalize(shadow_ray)
        hit = False
        for obj in self.objects:
            o,n = obj.intersect(shadow_ray,best_point)
            if o is not None:
                hit = True
        if hit:
            light = np.asarray([0,0,0])
        else:
            light = self.light_color
        # light = np.asarray(self.light_color)

        ambient = self.ambient
        obj = self.objects[best_i]
        material = obj.get_material()
        e = self.get_cam_loc() - best_point
        e = self.normalize(e)
        r = 2 * best_n * np.dot(shadow_ray,best_n) - shadow_ray
        r = self.normalize(r)
        part_spec = max(0,np.dot(e,r))**(1)
        spec = material.get_spec() * part_spec * light
        # print(spec)
        ca = ambient * material.get_diff()
        cd = light * material.get_diff() * np.dot(best_n, shadow_ray)
        cs = light*material.get_spec() * max(0,np.dot(e,r))**material.get_phong()
        spec = np.clip(spec,0,1)
        color = cs + ca + cd
        color = np.clip(color,0,1)
        return color
        # print(ob_inter)
        
        return self.objects[ob_inter[0][1]].get_color()
    def test_poly(self):
        t = np.asarray([3,-3,0])
        c = np.asarray(self.get_cam_loc())
        vec = np.asarray(t-c)
        vec = vec / la.norm(vec)
        print("vec",vec)
        self.objects[0].intersect(vec,self.get_cam_loc())

