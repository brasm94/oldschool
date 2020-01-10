#include "scene.h"
#define SAMPLES 10

Scene::Scene(){}
void Scene::setViewPort(double view_port_size_x2, double view_port_size_y2)
{
    view_port_size_x = view_port_size_x2;
    view_port_size_y = view_port_size_y2;
}
void Scene::addSphere(Sphere* s)
{
	objects.push_back(s);
}
void Scene::addTriangle(Triangle* t)
{
	objects.push_back(t);
}
void Scene::addCamera(Camera new_cam)
{
	cam = new_cam;
}
void Scene::setWindowSize(double x_min2,double x_max2,double y_min2,double y_max2,double z_win2)
{
    x_min = x_min2;
    x_max = x_max2;
    y_min = y_min2;
    y_max = y_max2;
    z_win = z_win2;
}
void Scene::addRandom(normal_distribution<double> nd,default_random_engine gen)
{
    dis = nd;
    generator = gen;
}
void Scene::addBGColor(vector<double> color)
{
    bg = color;
}
void Scene::addLightSource(LightSource LS)
{
    lightsources.push_back(LS);
}
void Scene::addAmbient(vector<double> amb1)
{
    amb_color = amb1;
}
vector<double> Scene::getCamLoc()
{
    return cam.getCameraLocation();
}
double Scene::getWindowPixels(vector<vector<vector<double>>> &pixels)
{
    double row_scale = (y_max - y_min) / view_port_size_y;
    double col_scale = (x_max - x_min) / view_port_size_x;
    double toff = 0.5;
    for (int row = 0; row < view_port_size_y ; row++)
    {
        for (int col = 0; col < view_port_size_x ; col++)
    	{
            double trow = (row + toff) * row_scale + y_min;// + step_size_y;
            double tcol = (col + toff) *  col_scale + x_min;// + step_size_x;
            // printf("[%f,%f]",trow,tcol);
            pixels[view_port_size_y - 1 - row][col][0] = tcol;
            pixels[view_port_size_y - 1 - row][col][1] = trow;
            pixels[view_port_size_y - 1 - row][col][2] = z_win;
        }
    }
    return row_scale;
}
vector<double> Scene::getRayVector(vector<double> &pixel)
{
    vector<double> a;
    // printf("%lf\n",dis(generator));
    // printf("ROW SCALE %f",row_scale);
    for (size_t i = 0; i < pixel.size();i++)
    {
        double r = dis(generator);
        // printf("%lf\n",r);
        // double r =0;
        // if(i == 2)
        //     r = 0;  
        a.push_back(pixel[i] + r  - cam.getCamFromIdx(i));
    }
    return a;
}
void Scene::makeBoundingBoxes()
{
    for(size_t i = 0 ; i < objects.size();i++)
    {
        vector<vector<double>> bounds = objects[i]->getBoundingBoxVals();
        BoundingBox * bb = new BoundingBox(bounds[0],bounds[1]);
        bb->addObject(objects[i]);
        bboxes.push_back(bb);
    }

}
void Scene::MakeAbsoluteBox()
{
    vector<double> mins = bboxes[0]->getMins();
    vector<double> maxes = bboxes[0]->getMaxes();
    for(size_t i = 0 ; i < bboxes.size();i++)
    {
        vector<double> tmin = bboxes[i]->getMins();
        vector<double> tmax = bboxes[i]->getMaxes();
        //MINS
        if(mins[0] > tmin[0])
            mins[0] = tmin[0];
        if(mins[1] > tmin[1])
            mins[1] = tmin[1];
        if(mins[2] > tmin[2])
            mins[2] = tmin[2];
        // MAXES
        if(maxes[0] < tmax[0])
            maxes[0] = tmax[0];
        if(maxes[1] < tmax[1])
            maxes[1] = tmax[1];
        if(maxes[2] < tmax[2])
            maxes[2] = tmax[2];
    }
    // printVector(mins);
    // printVector(maxes);
    broot = new BoundingBox(mins,maxes);
    broot->addChildren(bboxes);
    broot->makeChildren();
    // broot->isLowNULL();
}
vector<double> Scene::getIntersectBox(vector<double> &ray,vector<double> loc)
{
    // broot->isLowNULL();
    // printf("TEST ROOT\n");
    if(broot->doesIntersect(ray,loc) == -1)
        return {0,0,0};
    Object * tobj = broot->getIntersectRoot(ray,loc);
    if(tobj == NULL)
        return {0,0,0};
    return {1,1,1};
}
vector<double> Scene::getIntersect(vector<double> &ray,vector<double> loc,int depth,double snell)
{
    
    // vector<vector<double>> inter_objs;
    // vector<size_t> indexes;
    // vector<double> cLoc = loc;
    // for (unsigned int i = 0 ; i < objects.size();i++)
    // {
    //     vector<double> inter = objects[i]->intersect(ray,cLoc,0);
    //     if(inter[0] != 3.14) // this is stupid
    //     {
    //         inter_objs.push_back(inter);
    //         indexes.push_back(i);
    //     }
    // }
    // int closetIDX = -1;
    // double bdsf = -1;
    // for (size_t i = 0 ; i < inter_objs.size();i++)
    // {
    //     double dis = disBetweenPoints(inter_objs[i],cLoc);
    //     if(dis < bdsf || closetIDX == -1)
    //     {
    //         bdsf = dis;
    //         closetIDX = i;
    //     }
    // }
    // if(closetIDX == -1)
    // {
    //     return bg;
    // }
    Object * obj = broot->getIntersectRoot(ray,loc);
    if(obj == NULL)
        return bg;
    // vector<double> best_location = inter_objs[closetIDX];
    vector<double> best_location = obj->intersect(ray,loc,0);
    
    //Get Normal
    // vector<double> norm = objects[indexes[closetIDX]]->getNormal(best_location);
    vector<double> norm = obj->getNormal(best_location);

    normalizeVector(norm);
    // Get Light Imfo
    // Material mat = objects[indexes[closetIDX]]->getMaterial();
    Material mat = obj->getMaterial();

    vector<double> total2 = {0,0,0};
    vector<double> combo = {0,0,0};
    for(size_t j = 0;j<lightsources.size();j++)
    {
        combo = getColorWLight(ray,lightsources[j],best_location,norm,mat,depth,snell);
        total2 = { total2[0] + combo[0],total2[1] + combo[1],total2[2] + combo[2]};
    }
    double s = double(lightsources.size());
    total2 = {total2[0]/s,total2[1]/s,total2[2]/s};
    return total2;
}
vector<double> Scene::getColorWLight(vector<double> ray,LightSource ls,vector<double> best_location,vector<double> norm,Material mat,int depth,double snell)
{
    vector<double> lc = ls.getLightColor();
    vector<double> light_direction = ls.getLightDirection(best_location,{0,0,0});
    // printVector(light_direction);
    // printVector(loc);
    normalizeVector(light_direction);
    //See if is in shadow
    //
    double epi = .01;
    vector<double> moved_slightly = {best_location[0] + light_direction[0] * epi,
                                    best_location[1] + light_direction[1] * epi,
                                    best_location[2] + light_direction[2] * epi};
    vector<double> lc2 = {0,0,0};
    for(int z = 0; z < SAMPLES + SAMPLES;z++)
    {
        vector<double> rands = {0,0,0};
        for(int r = 0;r<3;r++)
            rands[r] = dis(generator);
        lc = ls.getLightColor();
        vector<double> tlight_direction = ls.getLightDirection(best_location,rands);
        normalizeVector(tlight_direction);
        Object * obj =  broot->getIntersectRoot(tlight_direction,moved_slightly);
        if(obj != NULL)
        {
             vector<double> dumb_location = obj->intersect(tlight_direction,moved_slightly,0);
            if(abs(disBetweenPoints(dumb_location,moved_slightly)) > .1)
            {
                if(obj->getMaterial().getMatType() != 2)
                {
                    lc = {0,0,0};
                }
                else
                {
                    lc = {.9,.9,.9};
                }
            }
            else
            {
                // printf("IM hitting the same box\n");
            }
        }
        // for (unsigned int i = 0 ; i < objects.size();i++)
        // {SAMPLES
        //     vector<double> inter = objects[i]->intersect(tlight_direction,moved_slightly,1);
        //     if(inter[0] != 3.14) // this is stupid
        //     {
        //         if(abs(disBetweenPoints(inter,moved_slightly)) > .1)
        //         {
        //             if(objects[i]->getMaterial().getMatType() != 2)
        //             {
        //                 lc = {0,0,0};
        //                 break;
        //             }
        //             else
        //             {
        //                 lc = {.9,.9,.9};
        //             }
        //         }
        //     }
        // }
        lc2 = {lc2[0] + lc[0],lc2[0] + lc[0],lc2[0] + lc[0]};
    }
    lc = {lc2[0]/SAMPLES,lc2[1]/SAMPLES,lc2[2]/SAMPLES};
    // lc = ls.getLightColor();
    // lc = {lc[0] + tls[0],lc[1] + tls[1],lc[2] + tls[2]};
    //Get Material
    
    vector<double> total = {1,1,1};
    double ddn = max(0.0,dotProduct(norm,light_direction));
    vector<double> refl = {2 * ddn * norm[0] - light_direction[0],2 * ddn * norm[1] - light_direction[1],2 * ddn * norm[2] - light_direction[2]};
    normalizeVector(refl);
    vector<double> cam_loc = cam.getCameraLocation();
    vector<double> r_ray = {cam_loc[0] - best_location[0],cam_loc[1] - best_location[1],cam_loc[2] - best_location[2]};
    normalizeVector(r_ray);
    double phong_term = max(0.0,dotProduct(refl,r_ray));
    double ph_c = mat.getPhongC();
    double part_spec = pow(phong_term,ph_c);
    double nl = max(0.0,dotProduct(norm,light_direction));
    vector<double> combo = {0,0,0};
    if(mat.getMatType() == 0 || depth == 0)
    {
        vector<double> mat_color = {0,0,0};
        vector<double> d = mat.getDiff();
        vector<double> s = mat.getSpec();
        //Combine 3 things
        vector<double> specular = {lc[0] * s[0] * part_spec,lc[1] * s[1] * part_spec,lc[2] * s[2] * part_spec};
        vector<double> diff = {lc[0] * d[0] * nl,lc[1] * d[1] * nl,lc[2] * d[2] * nl};
        vector<double> amb = amb_color;
        vector<double> scales = {.1,.7,.2};
        total = combine3(amb,diff,specular,scales);
        combo = total;
    }
    else if(mat.getMatType() == 1)
    {
        // printVector(refl);
        vector<double> mloc = {best_location[0] + refl[0] * epi,best_location[1] + refl[1] * epi,best_location[2] + refl[2] * epi};
        
        vector<double> rcolor = {0,0,0};
        int bcount = 0;
        for(int i = 0 ; i < SAMPLES;i++)
        {
            vector<double> ref2 = {refl[0] + dis(generator),refl[1] + dis(generator),refl[2] + dis(generator)};
            vector<double> prcolor = getIntersect(ref2,mloc,depth-1);
            if(compareVec(prcolor,bg) == 1)
                bcount++;
            rcolor = {rcolor[0] + prcolor[0],rcolor[1] + prcolor[1],rcolor[2] + prcolor[2]};
            // printVector(rcolor);
        }
        rcolor = {rcolor[0]/SAMPLES,rcolor[1]/SAMPLES,rcolor[2]/SAMPLES};
        vector<double> diff;
        vector<double> d = mat.getDiff();
        if(lc[0] == 0 && lc[1] == 0 && lc[2] == 0)
            combo = {0,0,0};
        else if(compareVec(rcolor,bg) == 1 || bcount > SAMPLES/2)
        {
            vector<double> mat_color = {0,0,0};
            vector<double> d = mat.getDiff();
            vector<double> s = mat.getSpec();
            //Combine 3 things
            vector<double> specular = {lc[0] * s[0] * part_spec,lc[1] * s[1] * part_spec,lc[2] * s[2] * part_spec};
            vector<double> diff = {lc[0] * d[0] * nl,lc[1] * d[1] * nl,lc[2] * d[2] * nl};
            vector<double> amb = amb_color;
            vector<double> scales = {.0,1.0,.3};
            combo = combine3(amb,diff,specular,scales);
        }
        else
        {
            combo = rcolor;
        }
    }
    else if(mat.getMatType() == 2)
    {
        // printf("SNELL %.2f\n",snell);
        double theta = dotProduct(norm,r_ray);
        double n1;
        double n2;
        if(snell == 1.0)
        {
            n1 = 1.0;
            n2 = mat.getRefra();
        }
        else
        {
            // printf("USING THIS\n");
            n1 = mat.getRefra();;
            n2 = 1.0;
            norm = {norm[0] * -1,norm[1] * -1,norm[2] * -1};
        }
        double left_part_scalar = (n1/n2);
        double right_part_scalar = (n1/n2) * cos(theta) - sqrt(1 + (n1/n2)*(n1/n2) * (cos(theta * cos(theta)) -1));
        vector<double> T = {left_part_scalar * ray[0] + right_part_scalar * norm[0],
                            left_part_scalar * ray[1] + right_part_scalar * norm[1],
                            left_part_scalar * ray[2] + right_part_scalar * norm[2]};
        double epi2 = 1;
        normalizeVector(T);

        vector<double> tloc = {best_location[0] + T[0] * epi2,best_location[1] + T[1] * epi2,best_location[2] + T[2] * epi2};
        // printf("I SHOULD BE INSIDE=");

        vector<double> tcolor = {0,0,0};
        for(int i = 0 ; i < SAMPLES;i++)
        {
            vector<double> TT = {T[0]+dis(generator),T[1]+dis(generator),T[2]+dis(generator)};
            normalizeVector(TT);
            vector<double> ptcolor = getIntersect(TT,tloc,depth-1);//,n2);
            tcolor = {ptcolor[0] + tcolor[0],ptcolor[1] + tcolor[1],ptcolor[2] + tcolor[2]};
        }
        tcolor = {tcolor[0]/SAMPLES,tcolor[1]/SAMPLES,tcolor[2]/SAMPLES};
        // Normal color Calculation
        vector<double> mat_color = {0,0,0};
        vector<double> d = mat.getDiff();
        vector<double> s = mat.getSpec();
        //Combine 3 things
        vector<double> specular = {lc[0] * s[0] * part_spec,lc[1] * s[1] * part_spec,lc[2] * s[2] * part_spec};
        vector<double> diff = {lc[0] * d[0] * nl,lc[1] * d[1] * nl,lc[2] * d[2] * nl};
        vector<double> amb = amb_color;
        vector<double> scales = {.0,1.0,.3};
        vector<double> ncolor = combine3(amb,diff,specular,scales);
        combo = {tcolor[0] * .6 + ncolor[0] * .4,tcolor[1] * .6 + ncolor[1] * .4,tcolor[2] * .6 + ncolor[2] * .4};
    // combo = tcolor;
}
    return combo;
}