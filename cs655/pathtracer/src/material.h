#pragma once
#include <vector>
using namespace std;

class Material
{
    private:
        vector<double> diff;
        vector<double> spec;
        vector<double> ks;
        double phong_con;
        int mat_type;
        double refra;
    public:
        Material(){};
        Material(int r,vector<double> diff,vector<double> spec,double ph);
        Material(vector<double> d,vector<double> specular,vector<double> ks, double p_con);
        vector<double> getDiff();
        vector<double> getSpec();
        // vector<double> getRef();
        vector<double> getKS();

        double getPhongC();
        int getMatType();
        double getRefra();
        void addRefra(double r);
        double getSeeThrough();
        
};