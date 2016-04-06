#include "mesh.h"
#include <GL/gl.h>
#include <stdio.h>
#include "header.h"

// OpenGL display function
void Mesh::display()
{
<<<<<<< HEAD
    glClear(GL_COLOR_BUFFER_BIT);

    float scaling=0.5;
    glColor3f(1.0,0.0,0.0);
    for( const TRIANGLE& t : triangle)
    {
        printf("!");
        glBegin(GL_TRIANGLES);
        glVertex3f(t.v[0].x*scaling,t.v[0].y*scaling,t.v[0].z*scaling);
        glVertex3f(t.v[1].x*scaling,t.v[1].y*scaling,t.v[1].z*scaling);
        glVertex3f(t.v[2].x*scaling,t.v[2].y*scaling,t.v[2].z*scaling);
        glEnd();
    }
    glFlush();
    /* TODO : draw mesh
    //puts("i will draw");
    if(flag){

        for(const TRIANGLE& triangle : mesh->triangle){
            //glBegin(GL_TRIANGLES);
                printf("%f, %f, %f\n", triangle.v[0].x, triangle.v[0].y, triangle.v[0].z);
                printf("%f, %f, %f\n", triangle.v[1].x, triangle.v[1].y, triangle.v[1].z);
                printf("%f, %f, %f\n", triangle.v[2].x, triangle.v[2].y, triangle.v[2].z);
                //glVertex3f(triangle.v[0].x, triangle.v[0].y, triangle.v[0].z);
                //glVertex3f(triangle.v[1].x, triangle.v[1].y, triangle.v[1].z);
                //glVertex3f(triangle.v[2].x, triangle.v[2].y, triangle.v[2].z);
            //glEnd();
        }
        printf("%d\n", mesh->triangle.size());
        flag = 0;
    }
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3f(-0.2, -0.2, 0.0);
    glVertex3f(0.2, -0.2, 0.0);
    glVertex3f(0.0, 0.2, 0.0);
    glEnd();
=======
    glClearColor(0.5,0.5,0.5,1);
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

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-size,size,-size,size,-size*scale,size*scale);

    glScalef(scale,scale,scale);
    glTranslatef(translationX,translationY,translationZ);
    glRotatef(twist,0.0,0.0,1.0);
    glRotatef(elevation,1.0,0.0,0.0);
    glRotatef(azimuth,0.0,1.0,0.0);
    glGetFloatv(GL_PROJECTION_MATRIX,projectionMatrix);

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
            Vector3 n2(-it->normal[i].x*projectionMatrix[0]-it->normal[i].y*projectionMatrix[4]-it->normal[i].z*projectionMatrix[8],-it->normal[i].x*projectionMatrix[1]-it->normal[i].y*projectionMatrix[5]-it->normal[i].z*projectionMatrix[9],-it->normal[i].x*projectionMatrix[2]-it->normal[i].y*projectionMatrix[6]-it->normal[i].z*projectionMatrix[10]);
            n2=n2/n2.getNorm();
            glNormal3f(n2.x,n2.y,n2.z);
            glVertex3fv(aVertex[it->vertexNo[i]-1]);
        }
        glEnd();
    }
>>>>>>> origin/master

    glFlush();
    */
    // swap buffer
    //SwapBuffers(/*HWND of child window*/window_main.get_child_window()->getGLContext()->getHDC());
}
