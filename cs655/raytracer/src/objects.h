#pragma once
#include <string>
#include <vector>
#include "mymath.h"
#include <math.h>
#include "material.h"
//#include "sphere.h"
using namespace std;

class Object 
{
	private:
		Material mat;
	public:
        Object();
		// Intersect returns the intersect location with the ray or {3.14,3.14,3.14}
		virtual vector<double> intersect(vector<double> &ray,vector<double> loc,int isShadow) = 0;
		virtual Material getMaterial() = 0;
		virtual vector<double> getNormal(vector<double> loc) = 0;
		virtual vector<vector<double>> getBoundingBoxVals() = 0;

};