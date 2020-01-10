#include <vector>
using namespace std;
// Dot Product 
double dotProduct(vector<double> a,vector<double> b);
vector<double> crossProduct(vector<double> a,vector<double> b);
// subtraction of A - B
void subVector(vector<double> a,vector<double> b,vector<double> &c);
void normalizeVector(vector<double> &a);
void printVector(vector<double> a);
double disBetweenPoints(vector<double> &a,vector<double> &b);
vector<int> make255(vector<double> &a);
vector<double> combine3(vector<double> a,vector<double> b,vector<double> c,vector<double> scales);
vector<double> add_refl(vector<double> total,vector<double> refl_color);
int compareVec(vector<double> a, vector<double> b);