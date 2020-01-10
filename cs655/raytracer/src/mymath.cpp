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
void normalizeVector(vector<double> &a)
{
    double mag = sqrt(dotProduct(a,a));
    // printf("MAG = %.2f\n",mag);
    for (size_t i = 0 ; i < a.size();i++)
        a[i] = a[i] / mag;
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
