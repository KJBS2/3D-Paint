#include "objwindow.h"
#include<string.h>
#include<QFile>
#include<QString>
#include<QGLWidget>
#include<GL/GL.h>
#include<GL/GLU.h>

ObjWindow::ObjWindow(QString _file,QWidget *parent) : QGLWidget(parent)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(idleFunc()));
    timer->start(10);


    memset(isKey,0,sizeof(isKey));


    file=_file;
    aVertex.clear();
    aNormal.clear();
    aFace.clear();
    osize=0;
    translationX=translationY=translationZ=0;
    elevation=azimuth=twist=0;
    scale=1;
    scaleStep=0.05;
    angleStep=2.0;
    translationStep=0.05;

    Camera = CAMERA();
    parse();
}
void ObjWindow::parse()
{
    QFile target(file);

    if(target.open(QIODevice::ReadOnly))
    {
        QTextStream in(&target);
        while(!in.atEnd())
        {
            QString input = in.readLine();
            QByteArray array = input.toLocal8Bit();
            char* line = array.data();
            int i;

            if(!strncmp(line,"v ",2))
            {
                Vector3 v;
                sscanf(line+2,"%f%f%f",&v.x,&v.y,&v.z);
                aVertex.push_back(v);
                if(v.getNorm()>osize)osize=v.getNorm();
            }
            else if(!strncmp(line,"vn ",3))
            {
                Vector3 v;
                sscanf(line+2,"%f%f%f",&v.x,&v.y,&v.z);
                aNormal.push_back(-v);
            }
            else if(!strncmp(line,"f ",2))
            {
                int t;
                Vector3 n;
                aFace.push_back(Face());
                const char *pt=line+2;
                while(1)
                {
                    sscanf(pt,"%d",&t);
                    aFace.back().vertexNo.push_back(t);
                    if(strstr(pt,"/"))
                    {
                        pt=strstr(pt,"/")+1;
                        if(strstr(pt,"/")&&strstr(pt,"/")<strstr(pt," "))
                        {
                            pt=strstr(pt,"/")+1;
                            sscanf(pt,"%d",&t);
                            n=aNormal[t-1];
                        }
                    }
                    aFace.back().normal.push_back(n);
                    if(strstr(pt," "))pt=strstr(pt," ")+1;
                    else break;
                    for(i='0';i<='9';++i)
                    {
                        char digit[2];
                        digit[0]=i;
                        digit[1]=0;
                        if(strstr(pt,digit))break;
                    }
                    if(i>'9') break;
                }
            }
        }
    }

}
void ObjWindow::initializeGL()
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
void ObjWindow::doDisplayMatrix()
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
void ObjWindow::doDisplayLightOn()
{
    GLfloat lightAmbient[]={0.5,0.5,0.5,1};
    GLfloat lightDiffuse[]={0.7,0.7,0.7,1};
    GLfloat lightSpecular[]={1,1,1,1};
    GLfloat lightPosition[]={0,0,-osize,0};
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
void ObjWindow::doDisplaySample()
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
            glVertex3fvv(aVertex[it->vertexNo[i]-1]);
        }
        glEnd();
    }
}

void ObjWindow::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void ObjWindow::paintGL() {
    initializeGL();
    doDisplayMatrix();
    doDisplayLightOn();
    doDisplaySample();
    glFlush();
}


void ObjWindow::idleFunc()
{

    if(isKey[Qt::Key_A])
        Camera.position = Camera.position - Camera.xAxis *(float)0.05;
    if(isKey[Qt::Key_D])
        Camera.position = Camera.position + Camera.xAxis *(float)0.05;
    if(isKey[Qt::Key_S])
        Camera.position = Camera.position - Camera.normal*(float)0.05;
    if(isKey[Qt::Key_W])
        Camera.position = Camera.position + Camera.normal*(float)0.05;

    this->repaint();
}


void ObjWindow::inputKey(QKeyEvent* key){
    isKey[key->key()]=1;
}

void ObjWindow::releaseKey(QKeyEvent* key){
    isKey[key->key()]=0;
}
