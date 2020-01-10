//// Include non opengl /////
#include <stdio.h>
#include <typeinfo>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>

/// Includes GL related /////
#include "GL/glew.h"
// #include "GL/gl.h"
#include "GLFW/glfw3.h"
// #include <glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

///My includes///

#include "particles.h"
#include "tank.h"

using namespace std;

#define SIZE 50


// Globals //
float dx = 0;
float dy = 0;
float dz = 0; 
float xtheta = 0.0f;
float ytheta = 0.0f;
float ztheta = 0.0f;
float vector_scale = .01f;
float color = 1.0f;
glm::vec3 velDir = glm::vec3(1,0,0);


static int N = SIZE;
static float width = 1024;
static float height = 768;
static long unsigned int tLength = N*N*N;
static GLfloat * particle_positions = new GLfloat[tLength * 3];    
static GLfloat * particle_colors = new GLfloat[tLength * 3];  


void updateVelVec()
{
    glm::mat4 rx = glm::rotate(glm::mat4(1.0f),glm::radians(xtheta),glm::vec3(1.0f,0.0f,0.0f));
    glm::mat4 ry = glm::rotate(glm::mat4(1.0f),glm::radians(ytheta),glm::vec3(0.0f,1.0f,0.0f));
    glm::mat4 rz = glm::rotate(glm::mat4(1.0f),glm::radians(ztheta),glm::vec3(0.0f,0.0f,1.0f));
    glm::mat4 allR = rz * ry * rx;
    glm::vec4 temp  = allR * glm::vec4(1.0f,0.0f,0.0f,0.0f);
    velDir = glm::vec3(temp);
    // printf("LAME %f %f %f \n",velDir.x,velDir.y,velDir.z);

}
string readFile(string filename)
{
    ifstream t(filename);
    stringstream ss;
    ss << t.rdbuf();
    string str = ss.str();
    return str;
}
static void error_callback(int error, const char* description)
{
    printf("Error: %s\n", description);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
       glfwSetWindowShouldClose(window, GLFW_TRUE);
    //Moving the Cube
    if(key == GLFW_KEY_W && (action == GLFW_REPEAT || action == GLFW_PRESS))
        dz += .5;
    if(key == GLFW_KEY_S && (action == GLFW_REPEAT || action == GLFW_PRESS))
        dz -= .5;
    if(key == GLFW_KEY_D && (action == GLFW_REPEAT || action == GLFW_PRESS))
        dx -= .5;
    if(key == GLFW_KEY_A && (action == GLFW_REPEAT || action == GLFW_PRESS))
        dx += .5;
    if(key == GLFW_KEY_R && (action == GLFW_REPEAT || action == GLFW_PRESS))
        dy -= .5;
    if(key == GLFW_KEY_F && (action == GLFW_REPEAT || action == GLFW_PRESS))
        dy += .5;
    //direction the direction of the source
    if(key == GLFW_KEY_H && (action == GLFW_REPEAT || action == GLFW_PRESS))
        {velDir = glm::vec3(-1.0f,0.0f,0.0f);}
    if(key == GLFW_KEY_K && (action == GLFW_REPEAT || action == GLFW_PRESS))
        {velDir = glm::vec3(1.0f,0.0f,0.0f);}
    if(key == GLFW_KEY_U && (action == GLFW_REPEAT || action == GLFW_PRESS))
        {velDir = glm::vec3(0.0f,1.0f,0.0f);}
    if(key == GLFW_KEY_J && (action == GLFW_REPEAT || action == GLFW_PRESS))
        {velDir = glm::vec3(0.0f,-1.0f,0.0f);}
    if(key == GLFW_KEY_O && (action == GLFW_REPEAT || action == GLFW_PRESS))
        {velDir = glm::vec3(0.0f,0.0f,1.0f);}
    if(key == GLFW_KEY_L && (action == GLFW_REPEAT || action == GLFW_PRESS))
        {velDir = glm::vec3(0.0f,0.0f,-1.0f);}
    if(key == GLFW_KEY_O && (action == GLFW_REPEAT || action == GLFW_PRESS))
        {velDir = glm::vec3(0.0f,0.0f,1.0f);}
    if(key == GLFW_KEY_L && (action == GLFW_REPEAT || action == GLFW_PRESS))
        {velDir = glm::vec3(0.0f,0.0f,-1.0f);}
    // The Magnitude of the source;
    if(key == GLFW_KEY_N && (action == GLFW_REPEAT || action == GLFW_PRESS))
        {vector_scale -= .01;}
    if(key == GLFW_KEY_M && (action == GLFW_REPEAT || action == GLFW_PRESS))
        {vector_scale += .01;}
    if(key == GLFW_KEY_V && (action == GLFW_REPEAT || action == GLFW_PRESS))
        {color *= -1;}
    if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}
GLuint doShaderStuff()
{
    string vertexShader = readFile("shaders/basic.vert");
    string fragShader = readFile("shaders/basic.frag");
    const char * vsc = vertexShader.c_str();
    const char * fsc = fragShader.c_str();
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vsc, NULL);
    glCompileShader(vs);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fsc, NULL);
    glCompileShader(fs);

    GLuint shader_programme = glCreateProgram();
    glAttachShader(shader_programme, fs);
    glAttachShader(shader_programme, vs);
    glLinkProgram(shader_programme);
    const int bufSize = 1024;
	char msgBuf[bufSize];
    glGetShaderInfoLog(fs,bufSize,NULL,msgBuf);
    printf("%s\n",msgBuf);

    GLint result = 0;
    glGetProgramiv(shader_programme,GL_LINK_STATUS,&result);
    if(result != GL_TRUE)
    {
        int maxLength;
        int length;
        glGetProgramiv(shader_programme,GL_INFO_LOG_LENGTH,&maxLength);
        char* log = new char[maxLength];
        glGetProgramInfoLog(shader_programme,maxLength,&length,log);
        printf("%s",log);
        printf("stupid linker\n");
    }
    return shader_programme;
}
GLFWwindow * initGlWindow(int width,int height)
{
    GLFWwindow *error = NULL;
    if (!glfwInit())
    {
        printf("The init was not created Correctly\n");
        return error;
    }
    //Set call back
    glfwSetErrorCallback(error_callback);
    //Create Window :D
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    GLFWwindow* window = glfwCreateWindow(width, height, "My Title", NULL, NULL);
    glfwSetKeyCallback(window, key_callback);
    if(!window)
    {
        printf("The Window was not created Correctly\n");
        glfwTerminate();
        // exit(-1);
        return error;
    }
    glfwMakeContextCurrent(window);
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        printf("Error: %s\n", glewGetErrorString(err));
        return error;
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glDepthFunc(GL_LESS);
    GLfloat size = 10;
    glPointSize(size);
    return window;
}


// static int X = SIZE; // grid x
// static int Y = SIZE; // grid y
// static int Z = SIZE;

void updateBuffers(Tank *t)
{
    // int i;
    float scale = 1.5f;
    float color = 0;
    int irow,icol,idepth;
    int i;
    for(float row = 0, irow = 0; row < N; row++,irow++)
    {
        for(float col = 0, icol = 0; col < N; col++,icol++)
        {
            for(float depth = 0, idepth = 0 ; depth < N; depth++,idepth++)
            {
                i = t->IX(row,col,depth);
                color = t->getDensity(row,col,depth);
                // color = t->getVeloX(row,col,depth);
                particle_positions[i*3+0] = row * scale;
                particle_positions[i*3+1] = col * scale;
                particle_positions[i*3+2] = depth * scale;
                particle_colors[i*3+0] = color;
                particle_colors[i*3+1] = 0.0f;
                particle_colors[i*3+2] = 1.0f - color;
            }
        }
    }
}
void addDyeVel(Tank * tank,int xS,int xE,int yS,int yE,int zS,int zE,float amountDye,float xv,float yv,float zv)
{
    for(int i = xS ; i < xE;i++)
    {
        for(int j = yS ; j < yE;j++)
        {
            for(int k = zS ; k < zE;k++)
            {
                // printf(" %i %i %i \n",i,j,k);
                tank->addDye(i,j,k,amountDye);
                tank->addVelocity(i,j,k,xv,yv,zv);
            }   
        }   
    }
}
int main(int argc, char** argv)
{

    // Init Glfw

    GLFWwindow * window = initGlWindow(width,height);
    if(!window)
    {
        printf("There was an Error in init of GL stuffs\n");
        return 0;
    }
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  
    GLuint shader_programme = doShaderStuff();

    printf("Shaer programme = %i\n",shader_programme);

    //Binding Vertexes(Positions) and Colors to buffers
    GLuint posiBuffer = 0;
    glGenBuffers(1, &posiBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, posiBuffer);
    glBufferData(GL_ARRAY_BUFFER, tLength * 3 * sizeof(GLfloat),NULL, GL_STREAM_DRAW);
    
    GLuint colorBuffer = 0;
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, tLength * 3 * sizeof(GLfloat),NULL, GL_STREAM_DRAW);

    //Stuff for screen size
    glm::vec2 screenSize = glm::vec2(640,480);
    printf("%f %f\n",screenSize[0],screenSize[1]);
    GLint ssID = glGetUniformLocation(shader_programme,"screenSize");
    if(ssID == -1)
    {
        printf("Shader loading screenSize - ss is incorrect\n");
    }

    glm::mat4 model = glm::mat4(1.f);
    glm::mat4 view = glm::lookAt(glm::vec3(120.0f,140.0f,140.0f),glm::vec3(0.f,0.f,0.f),glm::vec3(0.f,1.f,0.f));
    glm::mat4 trans = glm::translate(glm::mat4(1.0f),glm::vec3(-8.0f,0.-2.0f,-2.0f));
    glm::mat4 persp_part = glm::perspective(glm::radians(45.0f),4.0f/3.0f,0.1f,500.0f);
    glm::mat4 persp = persp_part * view;// * trans;
    glm::mat4 modelview = trans * model;
    GLint projID = glGetUniformLocation(shader_programme,"projection");
    if(projID == -1)
    {
        printf("Shader loading MAT4 - projection is incorrect\n");
    }
    GLint modelviewID = glGetUniformLocation(shader_programme,"modelview");
    if(modelviewID == -1)
    {
        printf("Shader loading MAT4 - modelview is incorrect\n");
    }
    GLuint posID = glGetAttribLocation(shader_programme,"pos");
    if(posID == -1)
    {
        printf("Shader loading posID - position is incorrect\n");
    }
    GLuint colorID = glGetAttribLocation(shader_programme,"color");
    if(colorID == -1)
    {
        printf("Shader loading colorID - Color is incorrect\n");
    }

    static float timestep = 1.0f;
    static float visc = 0.000001f;
    static float diff = 0.000001f;
    //

    Tank * t = new Tank(N,timestep,visc,diff);
    t->addRandVelo();
    int count = 0;
    
    int ss = 2;
    while (!glfwWindowShouldClose(window))
    {
        if(true)
        {
            if(true)
            {
                // t->addDye(N/2,N/2,N/2,5.0f);
                // t->addVelocity(N/2,N/2,N/2,velDir.x * vector_scale,velDir.y * vector_scale,velDir.z * vector_scale);
                addDyeVel(t,N/2-ss,N/2+ss,N/2-ss,N/2+ss,N/2-ss,N/2+ss,color,velDir.x * vector_scale,velDir.y * vector_scale,velDir.z * vector_scale);
            }
            t->takeStep();
            // t->printMatrix(N-2,2);
            // if(count % 1 == 0)
            //     t->printMatrix(N-2,2);
            // if(count % 100 == 0)
            //     printf("Count = %i\n",count);
            count++ ;
        }
        updateBuffers(t);
        // if(glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS)
        //     dz += .5;
        // if(glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS)
        //     dz -= .5;
        // if(glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS)
        //     dx -= .5;
        // if(glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS)
        //     dx += .5;
        // if(glfwGetKey(window,GLFW_KEY_R) == GLFW_PRESS)
        //     dy -= .5;
        // if(glfwGetKey(window,GLFW_KEY_F) == GLFW_PRESS)
        //     dy += .5;
        // if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
        //     glfwSetWindowShouldClose(window, GLFW_TRUE);

        glm::mat4 n_translate = translate(glm::mat4(1.0f),glm::vec3(dx,dy,dz));
        glm::mat4 t_trans = modelview * n_translate;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shader_programme);
        glUniformMatrix4fv(projID,1,GL_FALSE,&persp[0][0]);
        glUniformMatrix4fv(modelviewID,1,GL_FALSE,&t_trans[0][0]);
        // glUniform2fv(ssID,1,&screenSizes[0]);

        //Enable passing of the positions
        glEnableVertexAttribArray(posID);
        glBindBuffer(GL_ARRAY_BUFFER,posiBuffer);
        glBufferData(GL_ARRAY_BUFFER,tLength * 3 * sizeof(GLfloat),NULL,GL_STREAM_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, tLength * sizeof(GLfloat) * 3, particle_positions);
        glVertexAttribPointer(posID,3,GL_FLOAT,GL_FALSE,0,(void*)0);
        //Enable passing of the colors
        glEnableVertexAttribArray(colorID);
        glBindBuffer(GL_ARRAY_BUFFER,colorBuffer);
        glBufferData(GL_ARRAY_BUFFER,tLength * 3 * sizeof(GLfloat),NULL,GL_STREAM_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, tLength * sizeof(GLfloat) * 3, particle_colors);
        glVertexAttribPointer(colorID,3,GL_FLOAT,GL_FALSE,0,(void*)0);

        // Fraw the triabnle
        glDrawArrays(GL_POINTS,0,tLength);
        //IDK why you disable
        glDisableVertexAttribArray(posID);

        // pull to see if should close and stuff
        glfwPollEvents();
        // // put the stuff we've been drawing onto the display
        glfwSwapBuffers(window);
    }


    glfwDestroyWindow(window);
    return 0;
}