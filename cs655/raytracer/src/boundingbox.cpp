#include "boundingbox.h"
BoundingBox::BoundingBox()
{
    maxes = {0};
    mins = {0};

};
BoundingBox::BoundingBox(vector<double> minT,vector<double> maxesT)
{
    mins = minT;
    maxes = maxesT;
    depth = 0;
    low = NULL;
    high = NULL;
    obj = NULL;
}
void BoundingBox::setDepth(int d)
{
    depth = d;
}
void BoundingBox::addObject(Object * ob)
{
    obj = ob;
    if(obj == NULL)
        printf("STUPISDFSKLDMFS\n");
}
void BoundingBox::addSphere(Sphere * sp)
{
    obj = sp;
}
void BoundingBox::addTriangle(Triangle * tr)
{
    obj = tr;
}
vector<double> BoundingBox::getMins()
{
    return mins;
}
vector<double> BoundingBox::getMaxes()
{
    return maxes;
}
void BoundingBox::addChildren(vector<BoundingBox*> bb)
{
    children_boxes = bb;
}
int BoundingBox::isLower(int dim,double split)
{
    if(maxes[dim] <= split)
        return 0;
    else if(mins[dim] > split)
        return 1;
    else
    {
        if(maxes[dim] - split < split - mins[dim])
            return 1;
        return 0;
    }
    printf("NEED TO DEAL WITH OVERLAP");
    return 1;
}
void BoundingBox::makeChildren()
{
    if(depth > 10)
        return;
    if(children_boxes.size()  < 3) // for debuggin;
        return;
    double biggest = 0;
    int dim = 0;
    for(int i = 0;i<3;i++)
    {
        if(biggest < (maxes[i] - mins[i]))
        {
            biggest = (maxes[i] - mins[i]);
            dim = i;
        }
    }
    vector<BoundingBox*> lows;
    vector<BoundingBox*> highs;
    double split = mins[dim] + biggest/2;
    for(size_t i = 0 ; i < children_boxes.size();i++)
    {
        int tLOWER = children_boxes[i]->isLower(dim,split);
        // printf("tlower = %i = ",tLOWER);
        if(tLOWER == 0)
            lows.push_back(children_boxes[i]);
        else if (tLOWER == 1)
            highs.push_back(children_boxes[i]);
        else if (tLOWER == 2)
        {
            lows.push_back(children_boxes[i]);
            highs.push_back(children_boxes[i]);
        }
    }
    //lows new Dim
    vector<double> minlows = mins;
    vector<double> maxlows = maxes;
    maxlows[dim] = split;
    //High New Dim
    vector<double> minHigh = mins;
    vector<double> maxHigh = maxes;
    minHigh[dim] = split;
    // printVector(minlows);
    // return;
    low = new BoundingBox(minlows,maxlows);
    high = new BoundingBox(minHigh,maxHigh);
    // printf("")
    low->addChildren(lows);low->setDepth(depth+1);low->setBox();
    high->addChildren(highs);high->setDepth(depth+1);high->setBox();
    
    low->makeChildren();
    high->makeChildren();
    // printVector(low->getMins());
    // printVector(low->getMaxes());
    // printVector(high->getMins());
    // printVector(high->getMaxes());
}
/*
Return -1 if no interesection else returns tnear
*/
void BoundingBox::isLowNULL()
{
    if(low)
        printf("NOT NULL\n");
    else
        printf("YES NULL\n");

}
double BoundingBox::doesIntersect(vector<double> ray,vector<double> loc)
{
    if(maxes.size() != 3 || mins.size() != 3)
    {
        // printf("this is a problem\n");
        return -2;
    }
    double tnear = numeric_limits<double>::min();
    double tfar = numeric_limits<double>::max();
    for(int i = 0 ; i < 3 ; i++)
    {
        if(ray[i] == 0)
        {
            if(loc[i] < mins[i] || loc[i] > maxes[i])
                return -1;
        }
        double t1 = (mins[i] - loc[i]) / ray[i];
        double t2 = (maxes[i] - loc[i]) / ray[i];
        if(t1>t2) swap(t1,t2);
        if(t1 > tnear) tnear = t1;
        if(t2 < tfar) tfar = t2;
        if(tnear > tfar) return -1;
        if(tfar < 0) return -1;
    }
    // printf("INTERSECT DEPTH %i\n ",depth);
    return tnear;
}
Object* BoundingBox::getIntersectRoot(vector<double> ray,vector<double> loc)
{
    //BaseCase no more layers Down just bouding boxes with objects inside
    // printf("%u",low->getMins().size());
    // isLowNULL();
    // return low->getObject();
    // return new Triangle();
    if(!low)
    {
        if(children_boxes.size() != 0)
        {
            double bdsf = numeric_limits<double>::max();
            Object * bobj = NULL;
            for (size_t i = 0 ; i < children_boxes.size();i++)
            {
                Object * obj = children_boxes[i]->getObject();
                vector<double> tloc = obj->intersect(ray,loc,0);
                if(tloc[0] == 3.14)
                    continue;
                double dis = disBetweenPoints(tloc,loc);
                if(dis < bdsf)
                {
                    bdsf = dis;
                    bobj = obj;
                }
            }
            return bobj;
        }
            // return children_boxes[0]->getObject();
        return obj;// children_boxes[0]->getObject();// obj;//new Triangle();
    }
    // return getIntersectChildren(ray,loc);
    double inLow = low->doesIntersect(ray,loc);
    double inHigh = high->doesIntersect(ray,loc);
    if(inLow == -2 || inHigh == -2)
    {
        return  NULL;
    }
    if(inLow == -1 && inHigh == -1)
        return NULL;
    else if(inLow == -1)
    {
        // printf("HERE\n");
        if(high->getObject() != NULL)
            printf("STUPID");
        return high->getIntersectRoot(ray,loc);
    }
    else if(inHigh == -1)
    {
        // printf("HERE2\n");
        return low->getIntersectRoot(ray,loc);
    }
    else
    {   
        if(inLow < inHigh)
        {
            Object* tobj  = low->getIntersectRoot(ray,loc);
            if(tobj)
                return tobj;
            else
                return high->getIntersectRoot(ray,loc);
        }
        else
        {
            Object* tobj = high->getIntersectRoot(ray,loc);
            if(tobj)
                return tobj;
            else
                return low->getIntersectRoot(ray,loc);
        }
    }
}
vector<double> BoundingBox::getIntersectChildren(vector<double> ray,vector<double> loc)
{
    return {1,1,1};
}
Object * BoundingBox::getObject()
{
    return obj;
}
void BoundingBox::setBox()
{
    swap(mins,maxes);
    for(auto b : children_boxes)
    {
        vector<double> tmins = b->getMins();
        vector<double> tmaxes = b->getMaxes();
        for(int i = 0 ; i <3 ;i++)
        {
            if(tmins[i] < mins[i])
                mins[i] = tmins[i];
            if(tmaxes[i] > maxes[i])
                maxes[i] = tmaxes[i];
        }
    }
}