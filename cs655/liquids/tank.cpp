#include "tank.h"

Tank::Tank(int N2,float dt2,float visc2,float diffusion)
{
    N = N2;
    dt = dt2;
    visc = visc2;
    diff = diffusion;
    initFluid();
    return;
}
Tank::~Tank()
{
    //Free Velocities
    free(Vx);
    free(Vy);
    free(Vz);

    free(Vx0);
    free(Vy0);
    free(Vz0);
    // Free s and Density;
    // free(d0);
    // free(d);
    //Free position
    // free(x);
    // free(y);
    // free(z);
}
void Tank::initFluid()
{
    d = (float *)calloc(N*N*N,sizeof(float));
    d0 = (float *)calloc(N*N*N,sizeof(float));
    
    //Velocity Now
    Vx  = (float *)calloc(N*N*N,sizeof(float));
    Vy  = (float *)calloc(N*N*N,sizeof(float));
    Vz  = (float *)calloc(N*N*N,sizeof(float));
    Vx0 = (float *)calloc(N*N*N,sizeof(float));
    Vy0 = (float *)calloc(N*N*N,sizeof(float));
    Vz0 = (float *)calloc(N*N*N,sizeof(float));
    // x = (float *)calloc(N*N*N,sizeof(float));
    // y = (float *)calloc(N*N*N,sizeof(float));
    // z = (float *)calloc(N*N*N,sizeof(float));
}
int Tank::IX(int x,int y, int z)
{
    return (x + (y * N) + (z * N * N));
}
void Tank::addDye(int x,int y, int z,float amount)
{
    int index = IX(x,y,z);
    d[index] += amount;
}
void Tank::addVelocity(int x,int y, int z,float aVx,float aVy,float aVz)
{
    int index = IX(x,y,z);
    Vx[index] += aVx;
    Vy[index] += aVy;
    Vz[index] += aVz;
}
void Tank::setBoundary(int bequal,float * arr)
{
    int idx1;
    int idx2;
    //do y z plane left and right side;
    for(int yy = 0 ; yy <  N ; yy++) 
    {
        for(int zz = 0 ; zz <  N ; zz++)
        {
            arr[IX(0,yy,zz)] = (bequal == 1) ? -arr[IX(1,yy,zz)] : arr[IX(1,yy,zz)];
            arr[IX(N-1,yy,zz)] = (bequal == 1) ? -arr[IX(N-2,yy,zz)] : arr[IX(N-2,yy,zz)];
        }
    }
    // do yx plane front and back;
    for(int xx = 0;xx< N ;xx++)
    {
        for(int yy = 0; yy <  N ;yy++)
        {
            arr[IX(xx,yy,0)] = (bequal == 3) ? -arr[IX(xx,yy,1)] : arr[IX(xx,yy,1)];
            arr[IX(xx,yy,N-1)] = (bequal == 3) ? -arr[IX(xx,yy,N-2)] : arr[IX(xx,yy,N-2)];

        }
    }
    //do xz plane top and bottom
    for(int xx = 0;xx< N ;xx++)
    {
        for(int zz = 0 ; zz <  N ;zz++)
        {
            arr[IX(xx,0,zz)] = (bequal == 2) ? -arr[IX(xx,1,zz)] : arr[IX(xx,1,zz)];
            arr[IX(xx,N-1,zz)] = (bequal == 2) ? -arr[IX(xx,N-2,zz)] : arr[IX(xx,N-2,zz)];

        }
    }
    // do the 8 corners which the the average of the 3 voxels neighbors.
    // for copy and paste VVV
    // arr[IX()] = third * (arr[IX()] + arr[IX()] + arr[IX()]);
    float third = 1.0f / 3.0f;
    //location 0 0 0 
    arr[IX(0,0,0)] = third * (arr[IX(1,0,0)] + 
                              arr[IX(0,1,0)] + 
                              arr[IX(0,0,1)]);
    //location 0 0 1 
    arr[IX(0,0,N-1)] = third * (arr[IX(1,0,N-1)] + 
                                     arr[IX(0,1,N-1)] + 
                                     arr[IX(0,0,N-2)]);
    //location 0 1 0
    arr[IX(0,N-1,0)] = third * (arr[IX(1,N-1,0)] + 
                                     arr[IX(0,N-2,0)] + 
                                     arr[IX(0,N-1,1)]);
    //location 0 1 1
    arr[IX(0,N-1,N-1)] = third * (arr[IX(1,N-1,N-1)] + 
                                            arr[IX(0,N-2,N-1)] + 
                                            arr[IX(0,N-1,N-2)]);
    //location 1 0 0 
    arr[IX(N-1,0,0)] = third * (arr[IX(N-2,0,0)] + 
                                     arr[IX(N-1,1,0)] + 
                                     arr[IX(N-1,0,1)]);
    //location 1 0 1                                 
    arr[IX(N-1,0,N-1)] = third * (arr[IX(N-2,0,N-1)] + 
                                            arr[IX(N-1,1,N-1)] + 
                                            arr[IX(N-1,0,N-2)]);
    //location 1 1 0
    arr[IX(N-1,N-1,0)] = third * (arr[IX(N-2,N-1,0)] + 
                                            arr[IX(N-1,N-2,0)] + 
                                            arr[IX(N-1,N-1,1)]);
    //location 1 1 0
    arr[IX(N-1,N-1,N-1)] = third * (arr[IX(N-2,N-1,N-1)] + 
                                                   arr[IX(N-1,N-2,N-1)] + 
                                                   arr[IX(N-1,N-1,N-2)]);                                        
}
void Tank::linSolve(int bequal, float *x0,float *x,float a,float c,int iterations)
{
    float reciprocol = 1.0f / c;
    for(int k = 0; k < iterations;k++)
    {
        for(int zz = 1; zz < N - 1;zz++)
        {
            for(int yy = 1; yy < N - 1;yy++)
            {
                for(int xx = 1; xx < N - 1;xx++)
                {
                    x[IX(xx,yy,zz)] =
                    // the old plus the 6 neighbors;
                    (x0[IX(xx,yy,zz)] + a *(
                        x[IX(xx+1,yy,  zz  )] +
                        x[IX(xx-1,yy,  zz  )] +
                        x[IX(xx,  yy+1,zz  )] +
                        x[IX(xx,  yy-1,zz  )] +
                        x[IX(xx,  yy  ,zz+1)] +
                        x[IX(xx,  yy  ,zz-1)])) * reciprocol;
                }
            }
        }
        setBoundary(bequal,x);
    }
}
void Tank::diffuse(int bequal,float *x0,float *x,float amount,int iterations)
{
    float a = dt * amount * (N) * (N) *(N);
    // linSolve(bequal,x,x0,a,1+6 * a,iterations);
    // float reciprocol =
    for(int k = 0; k < iterations;k++)
    {
        for(int zz = 1; zz < N - 1;zz++)
        {
            for(int yy = 1; yy < N - 1;yy++)
            {
                for(int xx = 1; xx < N - 1;xx++)
                {
                    x[IX(xx,yy,zz)] =
                    // the old plus the 6 neighbors;
                    (x0[IX(xx,yy,zz)] + a *(
                        x[IX(xx+1,yy,  zz  )] +
                        x[IX(xx-1,yy,  zz  )] +
                        x[IX(xx,  yy+1,zz  )] +
                        x[IX(xx,  yy-1,zz  )] +
                        x[IX(xx,  yy  ,zz+1)] +
                        x[IX(xx,  yy  ,zz-1)])) / (1.0f + 6.0f*a);
                }
            }


        }
        setBoundary(bequal,x);
    }
}
void Tank::project()
{
    //resuing vX0 and vYo for pressure and divergence
    float * p = Vx0;
    float * div = Vy0;
    float h = 1.0f / N;
    for(int zz = 1; zz < N - 1;zz++)
    {
        for(int yy = 1; yy < N - 1;yy++)
        {
            for(int xx = 1; xx < N - 1;xx++)
            {
                //calculate the divergence for each voxel;
                div[IX(xx,yy,zz)] = -h*(
                    Vx[IX(xx+1,yy  ,zz  )] - Vx[IX(xx-1,yy  ,zz)] + 
                    Vy[IX(xx  ,yy+1,zz  )] - Vy[IX(xx  ,yy-1,zz)] + 
                    Vz[IX(xx   ,yy ,zz+1)] - Vz[IX(xx  ,yy  ,zz-1)])/3;
                p[IX(xx,yy,zz)] = 0;                
            }
        }   
    }
    setBoundary(0,div);
    setBoundary(0,p);
    int iterations = 4;
    linSolve(0,p,div,1,6,iterations);
    for(int zz = 1; zz < N - 1;zz++)
    {
        for(int yy = 1; yy < N - 1;yy++)
        {
            for(int xx = 1; xx < N - 1;xx++)
            {
                Vx[IX(xx,yy,zz)] -= (p[IX(xx+1,yy,zz)] - p[IX(xx-1,yy,zz)])/3/h;
                Vy[IX(xx,yy,zz)] -= (p[IX(xx,yy+1,zz)] - p[IX(xx,yy-1,zz)])/3/h;
                Vz[IX(xx,yy,zz)] -= (p[IX(xx,yy,zz+1)] - p[IX(xx,yy,zz-1)])/3/h;
			}
		}
	}
    setBoundary(1,Vx);
    setBoundary(2,Vy);
    setBoundary(3,Vz);
}
void Tank::advect(int bequal,float *x0,float *x,float *vxx,float *vyy,float *vzz)
{
    int i,j,k;
    float xx,yy,zz,dt0;
    float Nfloat = N;
    dt0 = dt*(Nfloat);

    // used as the corners
    int i0,j0,k0;
    int i1,j1,k1;
    //used for the scaling of interpolation
    float sx0,sy0,sz0;
    float sx1,sy1,sz1;
    //temp var for inter
    float xy0,xy1;
    for(k = 1 ;k < N - 1 ;k++)
    {
        for(j = 1 ;j < N - 1 ;j++)
        {
            for(i = 1 ;i < N - 1 ;i++)
            {
                xx = i - dt0 * vxx[IX(i,j,k)];
                yy = j - dt0 * vyy[IX(i,j,k)]; 
                zz = k - dt0 * vzz[IX(i,j,k)];
                //Check boundaries cant inter from outside the cube LOL
                if(xx < 0.5f) xx = 0.5f; if(xx > Nfloat + 0.5f) xx = Nfloat + 0.5f;
                if(yy < 0.5f) yy = 0.5f; if(yy > Nfloat + 0.5f) yy = Nfloat + 0.5f; 
                if(zz < 0.5f) zz = 0.5f; if(zz > Nfloat + 0.5f) zz = Nfloat + 0.5f; 
                // Getting the 6 offsets for the 8 corners
                i0 = (int)(floorf(xx));
                j0 = (int)(floorf(yy));
                k0 = (int)(floorf(zz));

                i1 = i0+1;
                j1 = j0+1;
                k1 = k0+1;
                // getting the interpolation scales
                sx1 = xx - i0;
                sy1 = yy - j0;
                sz1 = zz - k0;
                sx0 = 1 - sx1;                
                sy0 = 1 - sy1;
                sz0 = 1 - sz1;
                xy0 = sx0 * (sy0 * x0[IX(i0,j0,k0)] + sy1 * x0[IX(i0,j1,k0)]) + sx1 * (sy0 * x0[IX(i1,j0,k0)]+ sy1 * x0[IX(i1,j1,k0)]);
                xy1 = sx0 * (sy0 * x0[IX(i0,j0,k1)] + sy1 * x0[IX(i0,j1,k1)]) + sx1 * (sy0 * x0[IX(i1,j0,k1)]+ sy1 * x0[IX(i1,j1,k1)]);
                x[IX(i,j,k)] = sz0 * xy0 + sz1 * xy1;
                // printf("%f %f %f\n",x0[IX(i0,j0,k0)],x0[IX(i0,j1,k0)],x0[IX(i1,j0,k0)]);
                // printf("%f,%f,%f,%f,%f,%f   %f %f\n",sx0,sy0,sz0,sx1,sy1,sz1,xy0,xy1);
                    
            }
        }
    }
    setBoundary(bequal,x);
}

void Tank::takeStep()
{
// //START DIFFUSE SECTION
    // Swap Buffers via pointers :D
    SWAPFLOATPTR(Vx0,Vx);
    SWAPFLOATPTR(Vy0,Vy);
    SWAPFLOATPTR(Vz0,Vz);
    //Diffuse the Velocities
    diffuse(1,Vx0,Vx,visc,4);
    diffuse(2,Vy0,Vy,visc,4);
    diffuse(3,Vz0,Vz,visc,4);
    // if (Vx0[50] > 1000 || Vx0[50] < -1000)
    // {
    //     printf("1-here",Vx0[50]);
    //     exit(2);
    // }
    //project Vx and Vy are used for divergence and density
    project();
    // if (Vx0[50] > 1000 || Vx0[50] < -1000)
    // {
    //     printf("2-here \n");
    //     exit(2);
    // }
// //END DIFFUSE SECTION

//START ADVECT SECTION
    // Swap Buffers via pointers :D
    SWAPFLOATPTR(Vx0,Vx);
    SWAPFLOATPTR(Vy0,Vy);
    SWAPFLOATPTR(Vz0,Vz);
    //Advect the Velocities
    advect(1,Vx0,Vx,Vx0,Vy0,Vz0);
    advect(2,Vy0,Vy,Vx0,Vy0,Vz0);
    advect(3,Vz0,Vz,Vx0,Vy0,Vz0);
    // if (Vx0[50] > 1000 || Vx0[50] < -1000)
    // {
    //     printf("3-here \n");
    //     exit(2);
    // }
    //Vx0 and Vy0 are used for divergence and density
    project();
    // if (Vx0[50] > 1000 || Vx0[50] < -1000)
    // {
    //     printf("4-here \n");
    //     exit(2);
    // }
//END ADVECT SECTION

//START DIFFUSE DYE
    SWAPFLOATPTR(d0,d);
    diffuse(0,d0,d,diff,4);
//END DIFFUSE DYE

//START ADVECT DYE
    SWAPFLOATPTR(d0,d);
    advect(0,d0,d,Vx,Vy,Vz);
//START ADVECT DYE
// if (Vx0[50] > 1000 || Vx0[50] < -1000)
//     {
//         printf("5-here \n");
//         exit(2);
//     }

}
// void Tank::swap_float_ptr(float * xx0,float * xx1)
// {
//     // float * t = xx0;
//     // xx0 = xx1;
//     // xx1 = t;
//     swap(xx0,xx1);
// }

float Tank::getDensity(int x,int y,int z)
{
    // return Vx[IX(x,y,z)];
    return d[IX(x,y,z)];
}
float Tank::getVeloX(int x,int y,int z)
{
    // return Vx[IX(x,y,z)];
    return Vx[IX(x,y,z)];
}
void Tank::printMatrix(int slice,int t)
{
    float * lame;
    if(t == 0)
        lame = d;
    else if(t == 1)
        lame = Vx;
    else if(t == 2)
        lame = Vy;
    else if(t == 3)
        lame = Vz;
    for(int i = 0;i < N;i++)
    {
        for(int j = 0;j < N;j++)
        {
            printf("% f,",lame[IX(j,i,slice)]);
        }
        printf("\n");
    }
    printf("------------------\n\n");
}
void Tank::printPtrMatrix(int slice,float * arr)
{
    float * lame = arr;
    for(int i = 0;i < N;i++)
    {
        for(int j = 0;j < N;j++)
        {
            printf("% f,",lame[IX(j,i,slice)]);
        }
        printf("\n");
    }
    printf("------------------\n\n");
}
void Tank::addRandVelo()
{
    int i,j,k;
    for(k = 0 ;k < N  ;k++)
    {
        for(j = 0 ;j < N ;j++)
        {
            for(i = 0 ; i < N ; i++)
            {
                // t->addDye(i,j,k,4.0f);
                float a = (rand() % 100 /50.0f) - 1.0f;
                float b = (rand() % 100 /50.0f) - 1.0f;
                float c = (rand() % 100 /50.0f) - 1.0f;
                addVelocity(i,j,k,a/N,b/N,c/N);
            }
        }
    }
}


