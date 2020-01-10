#include "camera.h"
#include <string>
Camera::Camera(){};
Camera::Camera(double x_from,double y_from,double z_from,double x_at,double y_at,double z_at)
{
        lookFrom =  vector<double>{x_from,y_from,z_from};
        lookAt = vector<double>{x_at,y_at,z_at};
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
