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

class Mesh
{
public:
    void display();

public:
    vector<Vector3> aVertex,aNormal;
    vector<Face> aFace;
    GLfloat translationX,translationY,translationZ,translationStep=0.05;
    GLfloat twist,elevation,azimuth,angleStep=2.0;
    GLfloat scale,size,scaleStep;
    GLfloat projectionMatrix[16];
};





#endif // MESH_H_INCLUDED
