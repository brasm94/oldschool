#include "material.h"
Material::Material(vector<double> d,vector<double> specular,vector<double> ks2,double p_con)
{
    diff = d;
    spec = specular;
    phong_con = p_con;
    mat_type = 0;
    refra = 1;
    ks = ks2;
}
Material::Material(int r,vector<double> d,vector<double> s,double ph)
{
    diff = d;
    spec = s;
    phong_con = ph;
    mat_type = r;
    refra = 1;
}
vector<double> Material::getDiff(){return diff;}
vector<double> Material::getSpec(){return spec;}
double Material::getPhongC(){return phong_con;}
int Material::getMatType(){return mat_type;}
vector<double> Material::getKS(){return ks;};
// vector<double> Material::getRef(){return ref;}
double Material::getRefra(){return refra;}
void Material::addRefra(double r){refra = r;}
double Material::getSeeThrough(){return ks[3];}


