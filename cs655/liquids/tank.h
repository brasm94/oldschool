#pragma once
#include <glm/glm.hpp>
#include <stdio.h>
#include <algorithm>
#include <cstdlib>
#include <stdlib.h>

using namespace std;


#define SWAPFLOATPTR(x,y) {float *t=x;x=y;y=t;}

class Tank
{
    private:
        //Passed into constructort AKA user input
        int N;
        float diff;
        float dt;
        float visc;
        //used for solving fluids
        float * d0; // IDK what this one is for
        float * d;
        
        float * Vx;
        float * Vy;
        float * Vz;

        float * Vx0;
        float * Vy0;
        float * Vz0;
        //Position
        // float * x;
        // float * y;
        // float * z;

    public:
        Tank(){};
        Tank(int N2,float dt2,float visc2,float diffusion);
        ~Tank();

        //Setup
        void initFluid();
        int IX(int x,int y,int z);
        void addRandVelo();


        //Fluid Functions
        void setBoundary(int bequal,float * arr);
        void linSolve(int bequal, float *x,float *x0,float a,float c,int interations);
        void diffuse(int bequal,float *x,float *x0,float amount,int interations);
        void project();
        void advect(int bequal,float *x,float *x0,float *vxx,float *vyy,float *vzz);
        void takeStep();

        //Deprecated for Macro also didn't work
        void swap_float_ptr(float * xx0,float * xx1);

        //Adders
        void addDye(int x,int y, int z,float amount);
        void addVelocity(int x,int y, int z,float aVx,float aVy,float aVz);
        //Getters
        float getDensity(int x,int y,int z);
        float getVeloX(int x,int y,int z);
        //Debug Printers
        void printMatrix(int slice,int t);
        void printPtrMatrix(int slice,float * arr);



};