#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

#include<math.h>
#include<windows.h>
#include <GL/gl.h>
#include <vector>
using namespace std;

struct Vector3
{
    Vector3(float x=0,float y=0,float z=0)
    {
        this->x=x;
        this->y=y;
        this->z=z;
    }
    Vector3 operator-()
    {
        Vector3 r;
        r.x=-x;
        r.y=-y;
        r.z=-z;
        return r;
    }
    Vector3 operator+(Vector3 v)
    {
        Vector3 r;
        r.x=x+v.x;
        r.y=y+v.y;
        r.z=z+v.z;
        return r;
    }
    Vector3 operator-(Vector3 v)
    {
        Vector3 r;
        r.x=x-v.x;
        r.y=y-v.y;
        r.z=z-v.z;
        return r;
    }
    Vector3 operator/(GLfloat d)
    {
        Vector3 r;
        r.x=x/d;
        r.y=y/d;
        r.z=z/d;
        return r;
    }
    GLfloat getNorm()
    {
        return sqrt(x*x+y*y+z*z);
    }
    float x,y,z;
};

struct Face
{
    vector<int> vertexNo;
    vector<Vector3> normal;
};



inline void glVertex3fv(Vector3 &v)
{
    glVertex3f(v.x,v.y,v.z);
}

struct CAMERA{
    Vector3 position, normal, yAxis, xAxis;
    Vector3 baseNormal, baseYAxis, baseXAxis;
    float azimuth, elevation, twist;
    CAMERA() {
        position = Vector3(0,0,0);
        baseNormal = normal = Vector3(0,0,-1);
        baseXAxis = xAxis = Vector3(1,0,0);
        baseYAxis = yAxis = Vector3(0,1,0);
        azimuth = elevation = twist = 0;
    }
};

class Mesh
{
public:
    Mesh();
    void DoDisplay();
    void DoDisplayInit();
    void DoDisplayMatrix();
    void DoDisplayLightOn();
    void DoDisplayGridline();
    void DoDisplaySample();
    void DoDisplayString();


public:

    vector<Vector3> aVertex,aNormal;
    vector<Face> aFace;
    GLfloat translationX,translationY,translationZ,translationStep;
    GLfloat twist,elevation,azimuth,angleStep;
    GLfloat scale,size,scaleStep;
    GLfloat projectionMatrix[16];
    CAMERA Camera;
    int prvX,prvY;
};





#endif // MESH_H_INCLUDED
