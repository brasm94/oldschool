#include "scene.h"
#define SAMPLES 10

Scene::Scene(){}

void Scene::addSphere(Sphere* s)
{
	objects.push_back(s);
}
void Scene::addTriangle(Triangle* t)
{
	objects.push_back(t);
}
vector<double> Scene::getRay(int x,int y,bool rando)
{
	return cam.getRay(x,y,rando);
}
vector<double> Scene::getCameraLoc()
{
	return cam.getCameraLocation();
}
// vector<double> Scene::getDiffuse()
vector<double> Scene::intersect(Ray dray,vector<double> cLoc,int depth)
{

	vector<double> ray = dray.getDirection();
	if(depth == 0)
	{
		// printf("called\n");
		return bg_color;
	}
    int closetIDX = getInterObj(dray,cLoc,0);
    if(closetIDX == -1)
    {
        return bg_color;
    }
	// return {0,0,1};
	vector<double> inter = objects[closetIDX]->intersect(ray,cLoc,0);
	// printVector(inter);
	vector<double> norm = objects[closetIDX]->getNormal(inter);
	normalizeVector(norm);
	vector<double> tlight = light_sources[0].getLightDirection(inter);
	normalizeVector(tlight);
	vector<double> inv_ray = {ray[0] * -1,ray[1] * -1,ray[2] * -1};
	normalizeVector(inv_ray);
    double ddn = max(0.0,dotProduct(norm,inv_ray));
    vector<double> r_ray = {2 * ddn * norm[0] - inv_ray[0],2 * ddn * norm[1] - inv_ray[1],2 * ddn * norm[2] - inv_ray[2]};
	vector<double> ks = objects[closetIDX]->getMaterial().getKS();
	// mulVec(refl,-1);
	// printVector(ks);
    normalizeVector(r_ray);	
	// printVector(r_ray);
    // printVector(r_ray);
	
	//Normal color
	vector<double> bcolor = getBaseMLights(closetIDX,dray,cLoc);
	Material mat = objects[closetIDX]->getMaterial();
	// return bcolor;
	double r = drand48();
	double e = drand48();
	// printf("RANDOM = [%f]\n",e);
	if (e < ks[0])
	{
		return bcolor;
	}
	Ray nray;
	vector<double> rand_ray;
	if(r < ks[1])
	{
		rand_ray = getLambert(norm);
		normalizeVector(rand_ray);
		nray = Ray(rand_ray,dray.getMedium(),0);
		vector<double> mLoc = moveSlightly(rand_ray,inter);
		vector<double> rcolor = intersect(nray,mLoc,depth-1);
		return intersect(nray,mLoc,depth-1);
	}
	else if (r < ks[1] + ks[2])
	{
		rand_ray = getReflRay(r_ray,.2);
		normalizeVector(rand_ray);
		nray = Ray(rand_ray,dray.getMedium(),0);
		vector<double> mLoc = moveSlightly(rand_ray,inter);
		vector<double> rcolor = intersect(nray,mLoc,depth-1);
		return intersect(nray,mLoc,depth-1);
	}
	else if (r < ks[1] + ks[2] + ks[3])
	{
		double cosi = dotProduct(norm,ray); //c =theta
		double etai = 1;
        double etat =  mat.getRefra();
		if(cosi < 0)
		{
			cosi = -1 * cosi;
		}
		else
		{
			mulVec(norm,-1);
			swap(etai,etat);
		}
		double  eta = etai/etat;
		double k = 1 - eta * eta * (1 - cosi * cosi);
		if(k < 0)
		{
			printf("hmmm\n");
		}
		vector<double> left = ray;
		mulVec(left,eta);
		vector<double> right = norm;
		mulVec(right,eta * cosi - sqrt(k));
		addVector2(left,right);
		// printf("before = ");printVector(ray);
		// printf("after = ");printVector(left);
		rand_ray = left;
		normalizeVector(rand_ray);
		nray = Ray(rand_ray,etat,0);
		normalizeVector(rand_ray);
		vector<double> mLoc = moveSlightly(rand_ray,inter);
		// vector<double> rcolor = intersect(nray,mLoc,depth-1);
		// return {1,0,1};
		return intersect(nray,mLoc,depth-1);
	}
	else
	{
		return bcolor;
	}
	return bcolor;
}
// Still need to add shadows
vector<double> Scene::getBaseMLights(int closetIDX,Ray dray,vector<double> cLoc)
{
	vector<double> ray = dray.getDirection();
	vector<double> acolor = {0,0,0};
	vector<double> inter = objects[closetIDX]->intersect(ray,cLoc,0);
	// printVector(inter);
	vector<double> norm = objects[closetIDX]->getNormal(inter);
	normalizeVector(norm);
	for(size_t i = 0 ; i < light_sources.size();i++)
	{
		LightSource ls = light_sources[i];
		vector<double> tcolor = getBaseColor(closetIDX,dray,cLoc,ls);
		vector<double> tlight = ls.getLightDirection(inter);
		normalizeVector(tlight);
		mulVec(tlight,1);
		double ratio =  max(0.0,dotProduct(norm,tlight));
		// printf("RATIO = %f\n",ratio);
		mulVec(tcolor,ratio);
		// printVector(tcolor);
		addVector2(acolor,tcolor);
		// break;
	}
	maxVec(acolor,1);
	return acolor;
}
vector<double> Scene::getBaseColor(int closetIDX,Ray dray,vector<double> cLoc,LightSource ls)
{
	vector<double> ray = dray.getDirection();
	//Intersection and normal
	vector<double> inter = objects[closetIDX]->intersect(ray,cLoc,0);
	// printVector(inter);
	vector<double> norm = objects[closetIDX]->getNormal(inter);
	normalizeVector(norm);
	//Light
	vector<double> tlight = ls.getLightDirection(inter);
	normalizeVector(tlight);
    double ddn = max(0.0,dotProduct(norm,tlight));
    vector<double> refl = {2 * ddn * norm[0] - tlight[0],2 * ddn * norm[1] - tlight[1],2 * ddn * norm[2] - tlight[2]};
    normalizeVector(refl);	
	// Vec toward Camera
	vector<double> mLoc = moveSlightly(tlight,inter);
	Ray lray = Ray(tlight,dray.getMedium(),0);
	vector<double> lc = getLightColor(lray,mLoc,ls);
	// printVector(lc);
    vector<double> r_ray = subVector2(getCameraLoc(),inter);
    normalizeVector(r_ray);

	Material mat = objects[closetIDX]->getMaterial();
	vector<double> d = mat.getDiff();
	vector<double> s = mat.getSpec();

	double phong_term = max(0.0,dotProduct(refl,r_ray));
    double ph_c = mat.getPhongC();
	// printf("PHONG TERM %f\n",ph_c);
    double part_spec = pow(phong_term,ph_c);
    double nl = max(0.0,dotProduct(norm,tlight));
	vector<double> specular = {lc[0] * s[0] * part_spec,lc[1] * s[1] * part_spec,lc[2] * s[2] * part_spec};
	vector<double> diff = {lc[0] * d[0] * nl,lc[1] * d[1] * nl,lc[2] * d[2] * nl};
	vector<double> scales = {0.1,0.6,.3};
	vector<double> total = combine3(ambient,diff,specular,scales);
	return total;
    
}
vector<double> Scene::getLightColor(Ray ray,vector<double> loc,LightSource ls)
{
	int idx = getInterObj(ray,loc,1);
	if(idx == -1)
		return ls.getLightColor();
	return {0,0,0};
}
int Scene::getInterObj(Ray dray,vector<double> cLoc,int isShadow)
{
	vector<double> ray = dray.getDirection();
	vector<vector<double>> inter_objs;
    vector<size_t> indexes;
	for (size_t i = 0 ; i < objects.size();i++)
	{
		vector<double> inter = objects[i]->intersect(ray,cLoc,isShadow);
		if(inter[0] != INT_MAX)
		{
			double r = drand48();
			if(isShadow == 1 && objects[i]->getSeeOdd() > r)
				continue;
			inter_objs.push_back(inter);
			indexes.push_back(i);
		}
	}
	int closetIDX = -1;
    double bdsf = -1;
    for (size_t i = 0 ; i < inter_objs.size();i++)
    {
        double dis = disBetweenPoints(inter_objs[i],cLoc);
        if(dis < bdsf || closetIDX == -1)
        {
            bdsf = dis;
            closetIDX = indexes[i];
        }
    }
	return closetIDX;
}


Scene::Scene(string filename,int nrows,int ncols)
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
			bg_color = color;continue;
		}
		else if(tag.compare("AMBIENT") == 0)
		{
			vector<double> color = get3Doubles(iss);
			ambient = color;
		}
		else if(tag.compare("CAMERA") == 0)
		{
			double x_from;iss >> x_from;
			double y_from;iss >> y_from;
			double z_from;iss >> z_from;
			double x_at; iss >> x_at;
			double y_at; iss >> y_at;
			double z_at; iss >> z_at;
			double width;iss >> width;
			double height;iss >> height;
			Camera camT = Camera(x_from,y_from,z_from,x_at,y_at,z_at,ncols,nrows);
			cam = camT;
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
			light_sources.push_back(ls);
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
			light_sources.push_back(ls);
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
			light_sources.push_back(ls);
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
			objects.push_back(sph);
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
			objects.push_back(t);
		}
	}
}
Material Scene::readMaterial(istringstream &iss)
{
	string temp; iss >> temp;
	// if(temp.compare("MATERIAL_REFLECTIVE") == 0)
	// {
	// 	vector<double> diff = get3Doubles(iss);
	// 	vector<double> spec = get3Doubles(iss);
	// 	double ph; iss >> ph;
	// 	// vector<double> ks = get3Doubles(iss);
	// 	Material mat = Material(1,diff,spec,ph);
	// 	return mat;
	// }
	if(temp.compare("MATERIAL_REFRACTION") == 0)
	{
		vector<double> diff = get3Doubles(iss);
		vector<double> spec = get3Doubles(iss);
		vector<double> ks = get4Doubles(iss);
		double ph; iss >> ph;
		Material mat = Material(diff,spec,ks,ph);
		double refra; iss >> refra;
		printf("REFRA %.3f\n",refra);
		mat.addRefra(refra);
		return mat;
	}
	// printf("HERE %s\n",temp.c_str());
	vector<double> diff = get3Doubles(iss);
	vector<double> spec = get3Doubles(iss);
	vector<double> ks = get4Doubles(iss);
	double phong_con; iss >> phong_con;
	printf("%f\n",phong_con);
	// throw exception();
	// vector<double> ks = get3Doubles(iss);
	Material mat = Material(diff,spec,ks,phong_con);
	return mat;
}
Camera Scene::getCamera(istringstream &iss)
{
	double pos;
	vector<double> c;
	for (size_t i = 0 ; i <8;i++)
	{
		iss >> pos;
		c.push_back(pos);
	}
	Camera cam = Camera(c[0],c[1],c[2],c[3],c[4],c[5],c[6],c[7]);
	return cam;
}
