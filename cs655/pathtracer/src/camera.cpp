#include "camera.h"
#include <string>
#define FOV 90
Camera::Camera(){};
Camera::Camera(double x_from,double y_from,double z_from,double x_at,double y_at,double z_at,double width2,double height2)
{
        lookFrom =  vector<double>{x_from,y_from,z_from};
        lookAt = vector<double>{x_at,y_at,z_at};
        width = width2;
        height = height2;
}
double Camera::getCamFromIdx(size_t idx)
{
        // printf("DUDE %s",type(lookFrom));
        return lookFrom[idx];
}
vector<double> Camera::getCameraLocation()
{
        return lookFrom;
}
vector<double> Camera::getRay(int x,int y,bool rando)
{
        double r1 = (((double) rand() / RAND_MAX)-.5)/2;
        double r2 = (((double) rand() / RAND_MAX)-.5)/2;
        if(rando == false)
        {
                r1 = 0;
                r2 = 0;
        }
        double rx = (double(x) + .5 + r1)/width;
        double ry = (double(y) + .5 + r2)/height;
        double PSX = (2 * rx - 1) * tan(FOV/2);
        double PSY = (1 - 2 * ry) * tan(FOV/2);
        // printf("%f %f \n",PSX,PSY);
        vector<double> ray = {PSX,PSY,0-1};// , lookFrom);   
        normalizeVector(ray);            
        return ray;
}
