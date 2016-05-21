#ifndef OBJWINDOW_H
#define OBJWINDOW_H

#include <QWidget>
#include <vector>
#include <QtOpenGL>
#include <QString>
#include <GL/GL.h>
#include <GL/GLU.h>

using namespace std;

struct Vector3
{
    Vector3(float x=0,float y=0,float z=0)
    {
        this->x=x;
        this->y=y;
        this->z=z;
    }
    Vector3 operator*(float k) {
        Vector3 r;
        r.x = x * k;
        r.y = y * k;
        r.z = z * k;
        return r;
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
    Vector3 operator/(float d)
    {
        Vector3 r;
        r.x=x/d;
        r.y=y/d;
        r.z=z/d;
        return r;
    }
    Vector3 UnitVector()
    {
        return (*this) * (1.0/getNorm());
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

inline void glVertex3fvv(Vector3 &v){
    glVertex3f(v.x,v.y,v.z);
}
inline float InnerProduct(Vector3 A, Vector3 B) {
    return A.x * B.x + A.y * B.y + A.z * B.z;
}

inline Vector3 CrossProduct(Vector3 A, Vector3 B) {
    Vector3 r;
    r.x = A.y * B.z - A.z * B.y;
    r.y = A.z * B.x - A.x * B.z;
    r.z = A.x * B.y - A.y * B.x;
    return r;
}

inline Vector3 rotateVector(Vector3 target, Vector3 axis, float theta) {
    Vector3 parallel = axis * (InnerProduct(target, axis) / InnerProduct(axis, axis));
    Vector3 vertical = target - parallel;
    Vector3 cross = CrossProduct(axis, vertical);

    Vector3 result = (vertical.UnitVector() * cos(theta) + cross.UnitVector() * sin(theta)) * vertical.getNorm() + parallel;
    return result;
}


struct CAMERA{
    Vector3 position, normal, yAxis, xAxis;
    Vector3 baseNormal, baseYAxis, baseXAxis;
    float azimuth, elevation, twist;
    CAMERA() {
        position = Vector3(0,0,3);
        baseNormal = normal = Vector3(0,0,-1);
        baseXAxis = xAxis = Vector3(1,0,0);
        baseYAxis = yAxis = Vector3(0,1,0);
        azimuth = elevation = twist = 0;
    }
};

class ObjWindow:public QWidget
{
    Q_OBJECT
public:
    explicit ObjWindow(QString file,QWidget *parent = 0);

signals:

public slots:

private:
    void parse();
    //

    void paintEvent(QPaintEvent *);
    void doDisplayInit();
    void doDisplayMatrix();
    void doDisplayLightOn();
    void doDisplaySample();

    //file directory
    QString file;

    //model information
    vector<Vector3> aVertex,aNormal;
    vector<Face> aFace;
    GLfloat translationX,translationY,translationZ,translationStep;
    GLfloat twist,elevation,azimuth,angleStep;
    GLfloat scale,osize,scaleStep;
    GLfloat projectionMatrix[16];
    CAMERA Camera;
    // ex) aVector aFace Camera.. etc
};

#endif // OBJWINDOW_H
