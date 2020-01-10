#include "lightsource.h"

LightSource::LightSource(double x,double y,double z,double r,double g,double b,int t)
{
    direction = {x,y,z};
    color = {r,g,b};
    ltype = t;
}
LightSource::LightSource(double x,double y,double z,double rd,double r,double g,double b,int type)
{
    direction = {x,y,z};
    color = {r,g,b};
    ltype = type;
    radius = rd;
}
vector<double> LightSource::getLightDirection(vector<double> loc,vector<double> randos)
{
    if(ltype == 0)
    {
        return direction;
    }
    else if(ltype == 1)
    {
        return {direction[0] - loc[0],direction[1] - loc[1],direction[2] - loc[2]};
    }
    else
    {
        return {direction[0] + randos[0] - loc[0],direction[1] + randos[1] - loc[1],direction[2] + randos[2] - loc[2]};
    }

}
vector<double> LightSource::getLightDirection(vector<double> loc)
{
    return {direction[0] - loc[0],direction[1] - loc[1],direction[2] - loc[2]};
}
vector<double> LightSource::getLightColor()
{
    return color;
}
