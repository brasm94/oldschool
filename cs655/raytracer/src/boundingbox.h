#pragma once
#include <vector>
#include "objects.h"
#include "triangle.h"
#include "sphere.h"
#include <limits>
using namespace std;

class BoundingBox 
{
    private:
        vector<double> mins;
        vector<double> maxes;
        Object* obj;
        vector<BoundingBox *> children_boxes;
        BoundingBox * low;
        BoundingBox * high;
        int depth;
    public:
        BoundingBox();
        BoundingBox(vector<double> minT,vector<double> maxesT);
        void addObject(Object * ob);
        void addSphere(Sphere * sp);
        void addTriangle(Triangle * tr);
        void addChildren(vector<BoundingBox*> bb);
        void makeChildren();
        vector<double> getMaxes();
        vector<double> getMins();
        int isLower(int dim,double split);
        void setDepth(int d);
        double doesIntersect(vector<double> ray,vector<double> loc);
        vector<double>getIntersectChildren(vector<double> ray,vector<double> loc);
        Object*getIntersectRoot(vector<double> ray,vector<double> loc);
        void isLowNULL();
        Object * getObject();
        void setBox();
        
};