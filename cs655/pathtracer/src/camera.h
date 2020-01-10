#pragma once
#include <vector>
#include <math.h>
#include "mymath.h"
using namespace std;
#define NROWS 256
#define NCOLS 256

class Camera
{
	private:
		vector<double> lookFrom;
		vector<double> lookAt;
		double width;
		double height;
    public:
        Camera();
		Camera(double x_from,double y_from,double z_from,double x_at,double y_at,double z_at,double width,double height);	
		double getCamFromIdx(size_t idx);
		vector<double> getCameraLocation();
		vector<double> getRay(int x , int y,bool rando);

};