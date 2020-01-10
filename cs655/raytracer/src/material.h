#pragma once
#include <vector>
using namespace std;

class Material
{
    public:
        vector<double> diff;
        vector<double> spec;
        double phong_con;
        int mat_type;
        double refra;
        Material(){};
        Material(int r,vector<double> diff,vector<double> spec,double ph);
        Material(vector<double> d,vector<double> specular,double p_con);
        vector<double> getDiff();
        vector<double> getSpec();
        // vector<double> getRef();
        double getPhongC();
        int getMatType();
        double getRefra();
        void addRefra(double r);
};