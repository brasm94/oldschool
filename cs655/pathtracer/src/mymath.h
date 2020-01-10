#include <vector>
#include <sstream>
#include <fstream>
#include <math.h>
using namespace std;
// Dot Product 
double dotProduct(vector<double> a,vector<double> b);
vector<double> crossProduct(vector<double> a,vector<double> b);
// subtraction of A - B
void subVector(vector<double> a,vector<double> b,vector<double> &c);
vector<double> subVector2(vector<double> a,vector<double> b);
void normalizeVector(vector<double> &a);
void printVector(vector<double> a);
double disBetweenPoints(vector<double> &a,vector<double> &b);
vector<int> make255(vector<double> &a);
vector<double> combine3(vector<double> a,vector<double> b,vector<double> c,vector<double> scales);
vector<double> combine2(vector<double> a,vector<double> b);
vector<double> add_refl(vector<double> total,vector<double> refl_color);
int compareVec(vector<double> a, vector<double> b);
vector<double> get3Doubles(istringstream &iss);
vector<double> get4Doubles(istringstream &iss);
vector<double> getXDoubles(istringstream &iss);
vector<double> moveSlightly(vector<double> ray,vector<double> loc);
vector<double> getLambert(vector<double> normal);
void mulVec(vector<double> &ray,double m);
vector<double> getReflRay(vector<double> refl,double crazy);
void addVector2(vector<double> &a,vector<double> b);
vector<double> combine2(vector<double> a,vector<double> b,double rat);
void maxVec(vector<double> &ray,double max);





