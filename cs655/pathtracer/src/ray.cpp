#include "ray.h"

Ray::Ray(){}
Ray::Ray(vector<double> dir,double med,bool isShad)
{
    direction = dir;
    medium = med;
    isShadow = isShad;
}
vector<double> Ray::getDirection(){return direction;}
double Ray::getMedium(){return medium;}
bool Ray::getShadow(){return isShadow;}