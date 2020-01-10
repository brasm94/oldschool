#include <string>
#include <vector>
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "lightsource.h"
#include "objects.h"
#include "triangle.h"
#include "boundingbox.h"
#include <random>
#include <limits>
using namespace std;

class Scene 
{
		
		// Scene(double view_port_size_x, double view_port_size_y);
		//sphere will need to be changed to object and sphere polygon/triangle will inherit from object
	private:
		vector<Object*> objects;
		double view_port_size_x;
		double view_port_size_y;
		double x_min,x_max,y_min,y_max,z_win;
		Camera cam;
		vector<double> bg;
		vector<double> amb_color;
		vector<LightSource> lightsources;
		vector<BoundingBox *> bboxes;
		BoundingBox * broot;
		normal_distribution<double> dis;
		default_random_engine generator;
	public:
		Scene();
		void addSphere(Sphere* s);
		void addTriangle(Triangle* t);

		void addCamera(Camera cam);
		vector<double> getCamLoc();
		void setViewPort(double view_port_size_x, double view_port_size_y);
		void setWindowSize(double x_min,double x_max,double y_min,double y_max,double z_win);
		double getWindowPixels(vector<vector<vector<double>>> &pixels);
		void addLightSource(LightSource LS);
		vector<double> getRayVector(vector<double> &pixel);
		vector<double> getIntersect(vector<double> &ray,vector<double> loc,int depth = 5,double snell = 1.0);
		void addBGColor(vector<double> m);
		void addAmbient(vector<double> amb1);
		vector<double> getColorWLight(vector<double> ray,LightSource ls,vector<double> best_location,vector<double> normal,Material mat,int depth,double snell);
		void makeBoundingBoxes();
		void MakeAbsoluteBox();
		vector<double> getIntersectBox(vector<double> &ray,vector<double> loc);
		void addRandom(normal_distribution<double> nd,default_random_engine gen);
};

