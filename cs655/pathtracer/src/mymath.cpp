#include "mymath.h"
#include <math.h>
#include <string>
double dotProduct(vector<double> a,vector<double> b)
{
    double total = 0;
    for (size_t i = 0; i < b.size();i++)
        total += (a[i] * b[i]);
    return total;
}
void subVector(vector<double> a,vector<double> b,vector<double> &c)
{
    for(size_t i = 0; i < a.size();i++)
    {
        c[i] = a[i] - b[i];
    }
}
void mulVec(vector<double> &ray,double m)
{
    for(size_t i = 0 ; i < ray.size();i++)
        ray[i] = ray[i] * m;
}
void maxVec(vector<double> &ray,double max)
{
    for(size_t i = 0; i < ray.size();i++)
        ray[i] = min(ray[i],max);
}
vector<double> moveSlightly(vector<double> ray,vector<double> loc)
{
    double epi = .02;
    vector<double> nLoc;
    for(size_t i = 0 ; i < loc.size();i++)
        nLoc.push_back(loc[i] + ray[i] * epi);
    return nLoc;
}
vector<double> subVector2(vector<double> a,vector<double> b)
{
    vector<double> ne;
    for(size_t i = 0 ; i < a.size();i++)
        ne.push_back(a[i]-b[i]);
    return ne;
}
void addVector2(vector<double> &a,vector<double> b)
{
    for(size_t i = 0 ; i < a.size();i++)
        a[i] = a[i] + b[i];
}
void normalizeVector(vector<double> &a)
{
    double mag = sqrt(dotProduct(a,a));
    // printf("MAG = %.2f\n",mag);
    for (size_t i = 0 ; i < a.size();i++)
        a[i] = a[i] / mag;
}
vector<double> combine2(vector<double> a,vector<double> b)
{
    vector<double> avg;
    for(size_t i =0; i < a.size();i++)
        avg.push_back((a[i] + b[i])/2);
    return avg;
}
vector<double> combine2(vector<double> a,vector<double> b,double rat)
{
    vector<double> avg;
    for(size_t i =0; i < a.size();i++)
        avg.push_back((a[i] * rat + b[i]* (1.0-rat)));
    return avg;
}
void printVector(vector<double> a)
{
    printf("[");
    for (size_t i = 0; i < a.size();i++)
    {
        if (i != a.size()-1)
            printf("%f,",a[i]);
        else
            printf("%f",a[i]);
    }
    printf("]\n");
}
double disBetweenPoints(vector<double> &a,vector<double> &b)
{
    double total = 0;
    for (size_t i = 0 ; i < a.size();i++)
        total += (a[i] - b[i]) * (a[i] - b[i]);
    return sqrt(total);
}
vector<int> make255(vector<double> &a)
{
    vector<int> better;
    for (size_t i = 0; i < a.size();i++)
        better.push_back(int(a[i] * 255));
    return better;
}
vector<double> combine3(vector<double> a,vector<double> b,vector<double> c,vector<double> scales)
{
    vector<double> comb;
    for (size_t i = 0; i < a.size();i++)
        comb.push_back(max(0.0,a[i] * scales[0]+b[i] * scales[1]+c[i] * scales[2]));
    return comb;
}
vector<double> crossProduct(vector<double> a,vector<double> b)
{
    vector<double> cross;
    cross.push_back(a[1]*b[2] - a[2]*b[1]);
    cross.push_back(a[2]*b[0] - a[0]*b[2]);
    cross.push_back(a[0]*b[1] - a[1]*b[0]);
    return cross;
}
vector<double> add_refl(vector<double> total,vector<double> refl_color)
{
    double scal = .8;
    double uscal = 1 - scal;
    vector<double> new_color;
    for(size_t i = 0 ; i < total.size();i++)
    {
        new_color.push_back(total[i] * scal + refl_color[i] * uscal);
    }
    return new_color;
}
int compareVec(vector<double> a, vector<double> b)
{
    for(size_t i = 0;i < a.size();i++)
    {
        if(a[i] != b[i])
        return 0;
    }
    return 1;
}
vector<double> get3Doubles(istringstream &iss)
{
	double rgb;
	vector<double> color;
	for (size_t i = 0;i<3;i++)
	{
		iss >> rgb;
		color.push_back(rgb);
	}
	return color;
}
vector<double> get4Doubles(istringstream &iss)
{
	double rgb;
	vector<double> color;
	for (size_t i = 0;i<4;i++)
	{
		iss >> rgb;
		color.push_back(rgb);
	}
	return color;
}
vector<double> getXDoubles(istringstream &iss,int x)
{
	double rgb;
	vector<double> color;
	for (int i = 0;i<x;i++)
	{
		iss >> rgb;
		color.push_back(rgb);
	}
	return color;
}
vector<double> getLambert(vector<double> normal)
{
    double rx,ry,rz;
    double magnitude;
    do {
        rx = 2.0 * (double) rand() / RAND_MAX  - 1.0;
        ry = 2.0 * (double) rand() / RAND_MAX  - 1.0;
        rz = 2.0 * (double) rand() / RAND_MAX  - 1.0;
        magnitude = sqrt(rx*rx+ry*ry+rz*rz);
    } while(magnitude > 1);

    vector<double> ray_random = {rx/magnitude,ry/magnitude,rz/magnitude};
    if(dotProduct(normal,ray_random) < 0)
        mulVec(ray_random,-1);
    return ray_random;

}
vector<double> getReflRay(vector<double> refl,double crazy)
{
    vector<double> nray;
    for(size_t i = 0 ; i < refl.size();i++)
    {
        nray.push_back(refl[i] + ((double) rand() / RAND_MAX-.5) * crazy);
    }
    return nray;

}

