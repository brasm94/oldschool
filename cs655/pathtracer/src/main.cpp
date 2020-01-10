#include <vector>
#include "scene.h"

// Dimensions
#define NROWS 512
#define NCOLS 512
#define SAMPLES 5000


using namespace std;
void rayTrace(Scene s,int img[NROWS][NCOLS][3]);
void write_to_ppm(string filename,int img[NROWS][NCOLS][3]);
int main(int argc, char *argv[])
{
	string filename = "scenes/scene23.txt";
	Scene s = Scene(filename,NROWS,NCOLS);
	static int img[NROWS][NCOLS][3];
	rayTrace(s,img);
	write_to_ppm("outputs/scene23.ppm",img);
	return 0;
	
}
void rayTrace(Scene s , int img[NROWS][NCOLS][3])
{
	#pragma omp parallel for
	for(int i = 0 ; i < NROWS ; i++)
		{
			// if ( i % )
			printf("FINISHED ROW [%i] of [%i]\n",i,NROWS);
			for(int j = 0 ; j < NCOLS;j++)
			{
				// printf("%i %i\n",i,j);
				vector<double> ccolor = {0,0,0};
				for(int sa = 0 ; sa < SAMPLES;sa++)
				{
					vector<double> ray = s.getRay(j,i,false);
					// ray = {0,0,-1};
					normalizeVector(ray);
					Ray tray = Ray(ray,1.0,0);
					vector<double> color = s.intersect(tray,s.getCameraLoc(),10);
					addVector2(ccolor,color);

				}
				double inv = 1.0/SAMPLES;
				mulVec(ccolor,inv);
				vector<int> tcolor = make255(ccolor);
				img[i][j][0]  = tcolor[0];
				img[i][j][1]  = tcolor[1];
				img[i][j][2]  = tcolor[2];
			}
		}
}
void write_to_ppm(string filename,int img[NROWS][NCOLS][3])
{
	string headers = "P3\n" + to_string(NROWS) + " " + to_string(NCOLS) + "\n" + "255\n";
	string data = "";
	for (size_t row = 0 ;row < NROWS;row++)
	{
		for (size_t col = 0 ; col < NCOLS;col++)
		{
			data += to_string(img[row][col][0]) + " "; 
			data += to_string(img[row][col][1]) + " ";
			data += to_string(img[row][col][2]) + " ";
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
