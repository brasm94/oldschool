#include <vector>
#include "camera.h"
#include "sphere.h"
#include "scene.h"
#include "mymath.h"
#include <iostream>
#include <fstream>
#include "material.h"
#include "lightsource.h"
#include <sstream>
#include "objects.h"
#include "triangle.h"

// Dimensions
#define NROWS 256
#define NCOLS 256
#define SAMPLES 20

using namespace std;
void rayTrace(Scene scene,vector<vector<vector<double>>> pixels,double width);
void write_to_ppm(size_t row, size_t col,string filename,vector<vector<vector<int>>> ppm);
Scene createSceneFromFile(string filename);
vector<double> get3Doubles(istringstream &iss);
Material readMaterial(istringstream &iss);
vector<double> getXDoubles(istringstream &iss,int x);
int main(int argc, char *argv[])
{
	Scene s = createSceneFromFile("scenes/ref2.txt");
	
	s.setViewPort(NCOLS,NROWS);
	vector<vector<vector<double>>> pixels(NROWS, vector<vector<double>>(NCOLS, vector<double>(3)));
	s.setWindowSize(-7.5,7.5,-7.5,7.5,0);
	double width = s.getWindowPixels(pixels);
	s.makeBoundingBoxes();
	s.MakeAbsoluteBox();
	printf("MADE ALL CHILDREN\n");
	normal_distribution<double> dis(0.0,width/5);
	default_random_engine generator(time(0));
	s.addRandom(dis,generator);
	rayTrace(s,pixels,width);
	return 0;
	
}
void rayTrace(Scene scene,vector<vector<vector<double>>> pixels,double width)
{
	vector<vector<vector<int>>> ppm(NROWS, vector<vector<int>>(NCOLS, vector<int>(3)));
	// printf("%lf\n",dis(generator));
	for (size_t row = 0; row < pixels.size();row++)
	{
		for (size_t col = 0; col < pixels[row].size();col++)
		{
			vector<double> total = {0,0,0};
			for(int i = 0 ; i < SAMPLES ; i++)
			{
				vector<double> ray = scene.getRayVector(pixels[row][col]);
				// ray = {0,0,-1};
				normalizeVector(ray);
				// vector<double> color = scene.getIntersectBox(ray,scene.getCamLoc());
				vector<double> color = scene.getIntersect(ray,scene.getCamLoc(),3,1.0);
				total = {total[0] + color[0],total[1] + color[1],total[2] + color[2]};
			}
			total = {total[0]/SAMPLES,total[1]/SAMPLES,total[2]/SAMPLES};
			ppm[row][col] = make255(total);
		}
		printf("Finished row=[%lu] of [%lu]\n",row,pixels.size());
	}
	write_to_ppm(NROWS,NCOLS,"outputs/stupid3.ppm",ppm);
}
void write_to_ppm(size_t row, size_t col,string filename,vector<vector<vector<int>>> ppm)
{
	string headers = "P3\n" + to_string(row) + " " + to_string(col) + "\n" + "255\n";
	string data = "";
	for (size_t row = 0 ;row < ppm.size();row++)
	{
		for (size_t col = 0 ; col < ppm[row].size();col++)
		{
			data += to_string(ppm[row][col][0]) + " "; 
			data += to_string(ppm[row][col][1]) + " ";
			data += to_string(ppm[row][col][2]) + " ";
		}
		data += "\n";
	}
	string total = headers + data;
	ofstream myFile;
	myFile.open(filename);
	if(myFile.is_open())
	{
		myFile << total;
	}
	myFile.close();
	return;
}
Scene createSceneFromFile(string filename)
{
	Scene s = Scene();
	string line;
	ifstream myFile;
	myFile.open(filename);
	while(getline(myFile,line))
	{
		istringstream iss(line);
		string tag;
		iss >> tag;
		if(tag[0] == '%')
			continue;
		if (tag.compare("BACKGROUND") == 0)
		{
			vector<double> color = get3Doubles(iss);
			s.addBGColor(color);continue;
		}
		else if(tag.compare("AMBIENT") == 0)
		{
			vector<double> color = get3Doubles(iss);
			s.addAmbient(color);
		}
		else if(tag.compare("CAMERA") == 0)
		{
			double x_from;iss >> x_from;
			double y_from;iss >> y_from;
			double z_from;iss >> z_from;
			double x_at; iss >> x_at;
			double y_at; iss >> y_at;
			double z_at; iss >> z_at;
			Camera cam = Camera(x_from,y_from,z_from,x_at,y_at,z_at);
			s.addCamera(cam);
		}
		else if(tag.compare("DIRECTIONAL_LIGHT") == 0)
		{
			double x; iss >> x;
			double y; iss >> y;
			double z; iss >> z;
			double r; iss >> r;
			double g; iss >> g;
			double b; iss >> b;
			LightSource ls = LightSource(x,y,z,r,g,b,0);
			// printf("%lf,%lfa%lf,%lf,%lf,%lf",x,y,z,r,g,b);
			s.addLightSource(ls);
		}
		else if(tag.compare("POINT_LIGHT") == 0)
		{
			double x; iss >> x;
			double y; iss >> y;
			double z; iss >> z;
			double r; iss >> r;
			double g; iss >> g;
			double b; iss >> b;
			LightSource ls = LightSource(x,y,z,r,g,b,1);
			// printf("%lf,%lfa%lf,%lf,%lf,%lf",x,y,z,r,g,b);
			s.addLightSource(ls);
		}
		else if(tag.compare("AREA_LIGHT") == 0)
		{
			double x; iss >> x;
			double y; iss >> y;
			double z; iss >> z;
			double rd; iss >> rd;
			double r; iss >> r;
			double g; iss >> g;
			double b; iss >> b;
			LightSource ls = LightSource(x,y,z,rd,r,g,b,2);
			// printf("%lf,%lfa%lf,%lf,%lf,%lf",x,y,z,r,g,b);
			s.addLightSource(ls);
		}
		else if(tag.compare("SPHERE") == 0)
		{
			double x; iss >> x;
			double y; iss >> y;
			double z; iss >> z;
			double r; iss >> r;
			string temp_line;
			getline(myFile,temp_line);
			istringstream iss2(temp_line);
			Material mat = readMaterial(iss2);
			// printf("%lf,%lf,%lf,%lf\n",x,y,z,r);
			Sphere* sph = new Sphere(x,y,z,r,mat);
			s.addSphere(sph);
		}
		else if(tag.compare("TRIANGLE") == 0)
		{
			vector<double> pa = get3Doubles(iss);
			vector<double> pb = get3Doubles(iss);
			vector<double> pc = get3Doubles(iss);
			string temp_line;
			getline(myFile,temp_line);
			istringstream iss2(temp_line);
			Material mat = readMaterial(iss2);
			Triangle * t = new Triangle(pa,pb,pc,mat);
			s.addTriangle(t);
		}
	}
	return s;
}
Material readMaterial(istringstream &iss)
{
	string temp; iss >> temp;
	if(temp.compare("MATERIAL_REFLECTIVE") == 0)
	{
		vector<double> diff = get3Doubles(iss);
		vector<double> spec = get3Doubles(iss);
		double ph; iss >> ph;
		Material mat = Material(1,diff,spec,ph);
		return mat;
	}
	else if(temp.compare("MATERIAL_REFRACTION") == 0)
	{
		vector<double> diff = get3Doubles(iss);
		vector<double> spec = get3Doubles(iss);
		double ph; iss >> ph;
		Material mat = Material(2,diff,spec,ph);
		double refra; iss >> refra;
		printf("REFRA %.3f\n",refra);
		mat.addRefra(refra);
		return mat;
	}
	// printf("HERE %s\n",temp.c_str());
	vector<double> diff = get3Doubles(iss);
	vector<double> spec = get3Doubles(iss);
	double phong_con; iss >> phong_con;
	Material mat = Material(diff,spec,phong_con);
	return mat;
}
vector<double> get3Doubles(istringstream &iss)
{
	double rgb;
	vector<double> color;
	for (size_t i = 0;i<3;i++)
	{
		iss >> rgb;
		color.push_back(rgb);
	}
	return color;
}
vector<double> getXDoubles(istringstream &iss,int x)
{
	double rgb;
	vector<double> color;
	for (int i = 0;i< x;i++)
	{
		iss >> rgb;
		color.push_back(rgb);
	}
	return color;
}
Camera getCamera(istringstream &iss)
{
	double pos;
	vector<double> c;
	for (size_t i = 0 ; i < 6;i++)
	{
		iss >> pos;
		c.push_back(pos);
	}
	Camera cam = Camera(c[0],c[1],c[2],c[3],c[4],c[5]);
	return cam;
}