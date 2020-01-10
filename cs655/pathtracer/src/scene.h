
#include <vector>
#include "sphere.h"
#include "objects.h"
#include "triangle.h"
#include "camera.h"
#include "lightsource.h"
#include "mymath.h"
#include <sstream>
#include <fstream>
#include<bits/stdc++.h> 
#include "ray.h"



using namespace std;

class Scene 
{
		
		// Scene(double view_port_size_x, double view_port_size_y);
		//sphere will need to be changed to object and sphere polygon/triangle will inherit from object
	private:
		vector<Object*> objects;
		vector<double> bg_color;
		vector<double> ambient;
		Camera cam;
		vector<LightSource> light_sources;
	public:
		Scene();
		Scene(string filename,int nrows,int ncols);
		void addSphere(Sphere* s);
		void addTriangle(Triangle* t);
		Material readMaterial(istringstream &iss);
		Camera getCamera(istringstream &iss);
		vector<double> getCameraLoc();
		vector<double> getRay(int x,int y,bool rando);
		vector<double> intersect(Ray ray,vector<double> sLoc,int depth);
		int getInterObj(Ray ray,vector<double> cLoc,int isShadow);
		vector<double> getLightColor(Ray ray,vector<double> loc,LightSource ls);
		// vector<double> getDiffuse();
		vector<double> getBaseColor(int closetIDX,Ray rayy,vector<double> cLoc,LightSource ls);
		vector<double> getBaseMLights(int closetIDX,Ray ray,vector<double> cLoc);

		

};

