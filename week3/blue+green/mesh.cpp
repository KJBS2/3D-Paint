#include "mesh.h"
#include <GL/gl.h>
#include<stdio.h>

void Mesh::display()
{

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3f(1.0, 1.0, 0.0);
    glVertex3f(-1.0, 1.0, 0.0);
    glVertex3f(1.0, -1.0, 0.0);

    glEnd();
    glFlush();
}
