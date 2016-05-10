#include "mesh.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include "header.h"

// OpenGL display function
Mesh::Mesh()
{
    translationX=0;
    translationY=0;
    translationZ=0;
    elevation=0;
    azimuth=0;
    twist=0;
    scale=1;
    scaleStep=0.05;
    angleStep=2.0;
    translationStep=0.05;

    Camera = CAMERA();

    prvX = -1; prvY = -1;
}
void Mesh::DoDisplayInit()
{
    glClearColor(0,0,0,1); // Background Color

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);

    glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);

    glShadeModel(GL_SMOOTH);
}
void Mesh::DoDisplayMatrix()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1, 1, -1, 1, 1, 50);
    Camera.position+Camera.normal;
    Vector3 temp= Camera.position +Camera.normal;
    gluLookAt(Camera.position.x,Camera.position.y,Camera.position.z, temp.x,temp.y,temp.z, Camera.yAxis.x,Camera.yAxis.y,Camera.yAxis.z);
    glScalef(scale,scale,scale);
    glTranslatef(translationX,translationY,translationZ);
    glRotatef(twist,0.0,0.0,1.0);
    glRotatef(elevation,1.0,0.0,0.0);
    glRotatef(azimuth,0.0,1.0,0.0);
}
void Mesh::DoDisplayLightOn()
{
    GLfloat lightAmbient[]={0.5,0.5,0.5,1};
    GLfloat lightDiffuse[]={0.7,0.7,0.7,1};
    GLfloat lightSpecular[]={1,1,1,1};
    GLfloat lightPosition[]={0,0,-size,0};
    GLfloat materialAmbient[]={0.7,0.7,0.7,1};
    GLfloat materialSpecular[]={1,1,1,1};

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0,GL_AMBIENT,lightAmbient);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,lightDiffuse);
    glLightfv(GL_LIGHT0,GL_SPECULAR,lightSpecular);
    glLightfv(GL_LIGHT0,GL_POSITION,lightPosition);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);

    glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,materialAmbient);
    glMaterialfv(GL_FRONT,GL_SPECULAR,materialSpecular);
    glMaterialf(GL_FRONT,GL_SHININESS,128);
}
void Mesh::DoDisplaySample()
{
    glGetFloatv(GL_PROJECTION_MATRIX,projectionMatrix);
    for(vector<Face>::iterator it=aFace.begin();it!=aFace.end();++it)
    {
        glBegin(GL_POLYGON);
        if(it->vertexNo.size()<3)continue;
        for(unsigned int i=0;i<it->vertexNo.size();++i)
        {
            if(it->normal[i].x==0&&it->normal[i].y==0&&it->normal[i].z==0)
            {
                Vector3 ba=aVertex[it->vertexNo[1]-1]-aVertex[it->vertexNo[0]-1];
                Vector3 ca=aVertex[it->vertexNo[2]-1]-aVertex[it->vertexNo[0]-1];
                Vector3 n(ca.y*ba.z-ba.y*ca.z,ca.z*ba.x-ba.z*ca.x,ca.x*ba.y-ba.x*ca.y);
                it->normal[i]=n;
            }
            Vector3 n2(-it->normal[i].x*projectionMatrix[0]-it->normal[i].y*projectionMatrix[4]-it->normal[i].z*projectionMatrix[8]
                       ,-it->normal[i].x*projectionMatrix[1]-it->normal[i].y*projectionMatrix[5]-it->normal[i].z*projectionMatrix[9]
                       ,-it->normal[i].x*projectionMatrix[2]-it->normal[i].y*projectionMatrix[6]-it->normal[i].z*projectionMatrix[10]);
            n2=n2/n2.getNorm();
            glNormal3f(n2.x,n2.y,n2.z);
            glVertex3fv(aVertex[it->vertexNo[i]-1]);
        }
        glEnd();
    }
}



void Mesh::DoDisplay()
{
    DoDisplayInit();
    DoDisplayMatrix();
    DoDisplayLightOn();
    DoDisplaySample();
    glFlush();
}
void Mesh::CheckKeyboard()
{
    if(window_main.is_pressed['a'] || window_main.is_pressed['A'])
        Camera.position = Camera.position - Camera.xAxis *(float)0.05,this->DoDisplay();
    if(window_main.is_pressed['d'] || window_main.is_pressed['D'])
        Camera.position = Camera.position + Camera.xAxis *(float)0.05,this->DoDisplay();
    if(window_main.is_pressed['s'] || window_main.is_pressed['S'])
        Camera.position = Camera.position - Camera.normal*(float)0.05,this->DoDisplay();
    if(window_main.is_pressed['w'] || window_main.is_pressed['W'])
        Camera.position = Camera.position + Camera.normal*(float)0.05,this->DoDisplay();

}
