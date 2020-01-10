#pragma once
#include <vector>
using namespace std;

class Camera
{
	private:
		vector<double> lookFrom;
		vector<double> lookAt;
    public:
        Camera();
		Camera(double x_from,double y_from,double z_from,double x_at,double y_at,double z_at);	
		double getCamFromIdx(size_t idx);
		vector<double> getCameraLocation();
};