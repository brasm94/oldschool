#pragma once
#include <string>
#include <vector>
#include "mymath.h"
#include <math.h>
#include "material.h"
#include "objects.h"
#include<bits/stdc++.h> 

// #include "mymat"
//#include "sphere.h"
using namespace std;

class Triangle : public Object
{
	private:
		vector<double> normal;
		vector<double> backnormal;
		Material mat;
        vector<double> a;
        vector<double> b;
        vector<double> c;

	public:
		Triangle(){};
		Triangle(vector<double>a1,vector<double> b1,vector<double> c1,Material m);
		vector<double> intersect(vector<double> &ray,vector<double> loc,int isShadow);
		Material getMaterial();
		vector<double> getNormal(vector<double> loc);
		vector<double> plane_intersect(vector<double> &ray,vector<double> loc,int isShadow);
		vector<vector<double>> getBoundingBoxVals();
		double getSeeOdd();

		//sphere will need to be changed to object and sphere polygon/triangle will inherit from object
//		vector<Sphere> objects;
//		void addSphere(Sphere s);
};