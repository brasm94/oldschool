#pragma once
#include <string>
#include <vector>
#include "mymath.h"
#include <math.h>
#include "material.h"
#include "objects.h"
#include<bits/stdc++.h> 
//#include "sphere.h"
using namespace std;

class Sphere : public Object
{
	private:
        double x,y,z,r;
		Material mat;
	public:
		Sphere(double x , double y, double z, double r,Material m);
		vector<double> intersect(vector<double> &ray,vector<double> loc,int isShadow);
		Material getMaterial();
		vector<double> getNormal(vector<double> loc);
		vector<vector<double>> getBoundingBoxVals();
		//sphere will need to be changed to object and sphere polygon/triangle will inherit from object
//		vector<Sphere> objects;
		double getSeeOdd();

//		void addSphere(Sphere s);
};