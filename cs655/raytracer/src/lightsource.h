#pragma once
#include <vector>
#include <string>
#include <random>

using namespace std;

class LightSource 
{
    private:
        vector<double> direction;
        double radius;
        vector<double> color;
        int ltype;
    public:
        LightSource(){};
        //TYPE 0 Directional 1 = Point_LIGHT 2 = AREA_LIGHT
        LightSource(double x,double y,double z,double r,double g,double b,int type);
        LightSource(double x,double y,double z,double rd,double r,double g,double b,int type);
        vector<double> getLightDirection(vector<double> loc,vector<double> randos);
        vector<double> getLightColor();
};