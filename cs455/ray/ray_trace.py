from ppm import write_2_ppm
from ppm import write_2_ppm_2
from scene import scene
import numpy as np
from numpy import linalg as la
from material import material
def ray_trace(scene,x,y):
    points = scene.pixel2point()
    colors = np.asarray([],dtype=np.int)
    for p in points:
        vec = p - scene.get_cam_loc()
        vec = vec / la.norm(vec)
        col = scene.get_intersect(vec)
        # print(objs)
        colors = np.append(colors,col)
    colors = colors  * 255
    colors = colors.astype(int)
    write_2_ppm_2(colors,x,y,"testing.ppm")

def run():
    x_size = 512
    y_size = 512
    scene1 = scene(x_size,y_size)
    scene1.set_camera(0,0,10,0,0,0)
    scene1.set_window(0.0,-7.25,-7.25,7.25,7.25)
    scene1.add_light([10,0,0])
    cam_loc = scene1.get_cam_loc()
    mat = material([1,1,1],[1,1,1],4)
    scene1.add_sphere(3.5,0,-1,.5,mat)
    mat2 = material([1,0,0],[.5,1,.5],32)
    scene1.add_sphere(2,0,-1,.75,mat2)
    mat3 = material([0,1,0],[.5,1,.5],32)
    scene1.add_sphere(-6,0,0,3,mat3)
    p_t = [[3,-3,-4],[0,3,-1],[-3,-3,2]]
    mat4 = material([0,0,1],[1,1,1],32)
    scene1.add_poly(p_t,mat4)
    p_2t = [[-2,-1,1],[-2,-5,2],[-2,1,-3]]
    mat5 = material([1,1,0],[1,1,1],4)
    scene1.add_poly(p_2t,mat5)
    ray_trace(scene1,x_size,y_size)

if __name__ == "__main__":
    run()