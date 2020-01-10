#include "triangle.h"
Triangle::Triangle(vector<double> a1,vector<double> b1,vector<double> c1,Material m)
{
    a = a1;
    b = b1;
    c = c1;
    mat = m;
    vector<double> ba = {b[0] - a[0],b[1] - a[1],b[2] - a[2]};
    vector<double> ca = {c[0] - a[0],c[1] - a[1],c[2] - a[2]};

    normal = crossProduct(ca,ba);
    backnormal = crossProduct(ba,ca);
    normalizeVector(backnormal);
    normalizeVector(normal);
    // printf("NORMAL=");
    // printVector(normal);
    // printf("BNORMAL=");
    // printVector(backnormal);
}
vector<double> Triangle::plane_intersect(vector<double> &ray,vector<double> loc,int isShadow)
{
    double direction = dotProduct(ray,normal);
    vector<double> inter = {3.14,3.14,3.14};
    if (direction == 0)
    {
        return inter;
    }
    else if (isShadow == 0 && direction > 0)
        return inter;
    else if (isShadow == 1 && direction < 0)
        return inter;

    vector<double> difference = {0 - a[0],0 - a[1],0 - a[2]};
    double d = abs(dotProduct(difference,normal));
    double t =  -1* (dotProduct(loc,normal) + d )/ direction;
    if(t < 0)
        return inter;
    inter = {loc[0] + t * ray[0],loc[1] + t * ray[1],loc[2] + t * ray[2]};
    return inter;
}
vector<double> Triangle::intersect(vector<double> &ray,vector<double> loc,int isShadow)
{
    // printf("%i",isShadow);
    vector<double> n2use;
    n2use = normal;
    vector<double> p_inter = plane_intersect(ray,loc,isShadow);
    if (p_inter[0] == 3.14)
    {   
        // if(isShadow == 0)
            return p_inter;
    }
    vector<double> ba = {b[0] - a[0],b[1] - a[1],b[2] - a[2]};
    vector<double> cb = {c[0] - b[0],c[1] - b[1],c[2] - b[2]};
    vector<double> ac = {a[0] - c[0],a[1] - c[1],a[2] - c[2]};
    
    vector<double> qa = {p_inter[0] - a[0],p_inter[1] - a[1],p_inter[2] - a[2]};
    vector<double> qb = {p_inter[0] - b[0],p_inter[1] - b[1],p_inter[2] - b[2]};
    vector<double> qc = {p_inter[0] - c[0],p_inter[1] - c[1],p_inter[2] - c[2]};

    double t1 = dotProduct(crossProduct(ba,qa),n2use);
    double t2 = dotProduct(crossProduct(cb,qb),n2use);    
    double t3 = dotProduct(crossProduct(ac,qc),n2use);    
    vector<double> no_inter = {3.14,3.14,3.14};
    if (t1 > 0 || t2 > 0 || t3 > 0)
    {
        // if(isShadow == 0)
        // {
            return no_inter;
        // }
    }
    // if(isShadow == 0)
    return p_inter;

    // n2use = normal;
    // vector<double> p_inter2 = plane_intersect(ray,loc,isShadow);
    // // vector<double> ca = {c[0] - a[0],c[1] - a[1],c[2] - a[2]};
    // // vector<double> bc = {b[0] - c[0],b[1] - c[1],b[2] - c[2]};
    // // vector<double> ab = {a[0] - b[0],a[1] - b[1],a[2] - b[2]};
    // // qa = {p_inter2[0] - a[0],p_inter2[1] - a[1],p_inter2[2] - a[2]};
    // // qb = {p_inter2[0] - b[0],p_inter2[1] - b[1],p_inter2[2] - b[2]};
    // // qc = {p_inter2[0] - c[0],p_inter2[1] - c[1],p_inter2[2] - c[2]};
    // // t1 = dotProduct(crossProduct(ba,qa),n2use);
    // // t2 = dotProduct(crossProduct(cb,qb),n2use);    
    // // t3 = dotProduct(crossProduct(ac,qc),n2use); 
    // // if(isShadow)
    // //     printf("%.2f,%.2f,%.2f\n",t1,t2,t3);
    // if(p_inter2[0] != 3.14)
    //     printVector(p_inter2);
    // return no_inter;
    // if (t1 > 0 || t2 > 0 || t3 > 0)
    // // printVector(p_inter2);
    // return p_inter2;
    
}

Material Triangle::getMaterial()
{
    return mat;
}
vector<double> Triangle::getNormal(vector<double> loc)
{
    // printf("TN=%.2f,%.2f,%.2f\n",normal[0],normal[1],normal[2]);
    return normal;
}
vector<vector<double>> Triangle::getBoundingBoxVals()
{
    vector<double> mins = a;
    vector<double> maxs = a;
    vector<vector<double>> bounds;
    for(int i = 0;i<3;i++)
    {
        //Check Mins First
        if(a[i] < mins[i])
            mins[i] = a[i];
        if(b[i] < mins[i])
            mins[i] = b[i];
        if(c[i] < mins[i])
            mins[i] = c[i];
        //check Maxs
        if(a[i] > maxs[i])
            maxs[i] = a[i];
        if(b[i] > maxs[i])
            maxs[i] = b[i];
        if(c[i] > maxs[i])
            maxs[i] = c[i];
    }
    bounds.push_back(mins);
    bounds.push_back(maxs);
    return bounds;
}