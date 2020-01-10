#include "sphere.h"
Sphere::Sphere(double x1 , double y1, double z1, double r1,Material m)
{
    x = x1;
    y = y1;
    z = z1;
    r = r1;
    mat = m;
}
vector<double> Sphere::intersect(vector<double> &ray,vector<double> loc,int isShadow)
{
    /// Need to add check to see if camera is in sphere
    vector<double> oc = {x-loc[0],y-loc[1],z-loc[2]};
    vector<double> not_found = {INT_MAX,INT_MAX,INT_MAX};
    // normalizeVector(oc);
    double dis_to_sp = sqrt(dotProduct(oc,oc)); // for Checking to see if it is inside
    // normalizeVector(oc);
    double tca = dotProduct(oc,ray);
    bool inside = false;
    if (dis_to_sp < r)
    {
        // printf("IM INSIDE");
        inside = true;
    }
    if (tca < 0 && !inside)
    {
        // printf("This is not inside and no intersect\n");
        return not_found;
    }
    double d2 = dotProduct(oc,oc) - tca * tca;
    double thc2 = r*r - d2;
    if (thc2 < 0)
    {
        // printf("THC2 < 0 , THC2 == [%.3f]\n",thc2);
        return not_found;
    }
    // printf("\n");
    double t;
    if (inside)
    {
        // printf("im inside\n");
        t = tca + sqrt(thc2);
        // double tp = tca + sqrt(thc2);
        // double tm = tca - sqrt(thc2);
        // printf("DOUBLES %.2f %.2f\n",tp,tm);
    }
    else
    {
        // printf("this one\n");
        t = tca - sqrt(thc2);
    }
    // double t11 = tca + sqrt(thc2);
    // double t22 = tca - sqrt(thc2);
    // if(t11 > t22)
    //     printf("T11 is greater\n");
    // if (t < 0)
    //     printf("IM NEG\n");
    vector<double> inter = {loc[0] + ray[0] * t,loc[1] + ray[1] * t,loc[2] + ray[2] * t};
    return inter;
}
Material Sphere::getMaterial()
{
    return mat;
}
vector<double> Sphere::getNormal(vector<double> loc)
{
    vector<double> norm = {(loc[0]-x)/r,(loc[1]-y)/r,(loc[2]-z)/r};
    // printf("SN=%.2f,%.2f,%.2f\n",norm[0],norm[1],norm[2]);
    return norm;
}
vector<vector<double>> Sphere::getBoundingBoxVals()
{
    vector<vector<double>> bounds;
    vector<double> mins;
    vector<double> maxs;
    mins = {x-r,y-r,z-r};
    maxs = {x+r,y+r,z+r};
    bounds.push_back(mins);
    bounds.push_back(maxs);
    return bounds;
}
double Sphere::getSeeOdd()
{
    return mat.getSeeThrough();
}
