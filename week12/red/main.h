/**
@file main.h
@date ~2016-05-20
@brief Main header file
*/

#ifndef __MAIN_H__
#define __MAIN_H__

#include<map>
#include<vector>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"tinyfiledialogs.h"

#ifdef __APPLE__
#include<ApplicationServices/ApplicationServices.h>
#include<GLUT/glut.h>
#else
#include<GL/glut.h>
#endif

using namespace std;

/**
@def KIST_MODE_VIEW
@date ~2016-05-20
@brief Mode: view
@def KIST_MODE_SELECT
@date ~2016-05-20
@brief Mode: select
*/
#define KIST_MODE_VIEW      0
#define KIST_MODE_SELECT    1

/**
@def KIST_MOUSE_LEFT
@date ~2016-05-20
@brief Mouse: left
@def KIST_MOUSE_RIGHT
@date ~2016-05-20
@brief Mouse: right
@def KIST_MOUSE_BOTH
@date ~2016-05-20
@brief Mouse: both
@def KIST_MOUSE_SCROLLUP
@date ~2016-05-20
@brief Mouse: scroll up
@def KIST_MOUSE_SCROLLDOWN
@date ~2016-05-20
@brief Mouse: scroll down
*/
#define KIST_MOUSE_LEFT         0x0010
#define KIST_MOUSE_RIGHT        0x0020
#define KIST_MOUSE_BOTH         0x0030
#define KIST_MOUSE_SCROLLUP     0x0003
#define KIST_MOUSE_SCROLLDOWN   0x0004

/**
@var WindowWidth
@date ~2016-05-20
@brief Main window width
@var WindowHeight
@date ~2016-05-20
@brief Main window height
@var maxZ
@date 2016-05-25
@brief Max z value
*/
const int WindowWidth=600,WindowHeight=600,maxZ=50;

/**
@todo
*/
GLfloat lightAmbient[]={0.3,0.3,0.3,1};
GLfloat lightDiffuse[]={0.6,0.6,0.6,1};
GLfloat lightSpecular[]={1,1,1,1};
GLfloat lightPosition[]={0,0,-maxZ,0};
GLfloat materialAmbient[]={0.4,0.4,0.4,1};
GLfloat materialSpecular[]={1,1,1,1};

/**
@var dialogTitle
@date ~2016-05-20
@brief Load dialog title
@var filterPatterns
@date ~2016-05-20
@brief Load dialog pattern
@var filterDescription
@date ~2016-05-20
@brief Load dialog description
*/
const char *dialogTitle="Load",*filterPatterns[]={"*.obj"},*filterDescription="Object files (*.obj)";

/**
@date ~2016-05-20
@brief 3D vector structure
*/
struct Vector3
{
    /**
    @date ~2016-05-20
    @brief Construct a Vector3 instance.
    @param [in] x x-axis value
    @param [in] y y-axis value
    @param [in] z z-axis value
    @return Constructed Vector3 instance
    */
    Vector3(GLfloat x=0,GLfloat y=0,GLfloat z=0)
    {
        this->x=x;
        this->y=y;
        this->z=z;
    }

    /**
    @date ~2016-05-20
    @brief Get opposite signed instance of this.
    @par Parameters
        None
    @return Opposite signed Vector3 instance
    */
    Vector3 operator-()
    {
        Vector3 r;
        r.x=-x;
        r.y=-y;
        r.z=-z;
        return r;
    }

    /**
    @date ~2016-05-20
    @brief Add a parameter instance to this.
    @param [in] v Operand
    @return Added Vector3 instance
    */
    Vector3 operator+(Vector3 v)
    {
        Vector3 r;
        r.x=x+v.x;
        r.y=y+v.y;
        r.z=z+v.z;
        return r;
    }

    /**
    @date ~2016-05-20
    @brief Subtract a parameter instance from this.
    @param [in] v Operand
    @return Subtracted Vector3 instance
    */
    Vector3 operator-(Vector3 v)
    {
        Vector3 r;
        r.x=x-v.x;
        r.y=y-v.y;
        r.z=z-v.z;
        return r;
    }

    /**
    @date ~2016-05-20
    @brief Multiply float value to this.
    @param [in] m Multiplier
    @return Multiplied Vector3 instance
    */
    Vector3 operator*(GLfloat m)
    {
        Vector3 r;
        r.x=x*m;
        r.y=y*m;
        r.z=z*m;
        return r;
    }

    /**
    @date ~2016-05-20
    @brief Divide float value to this.
    @param [in] d Diviser
    @return Divided Vector3 instance
    */
    Vector3 operator/(GLfloat d)
    {
        Vector3 r;
        r.x=x/d;
        r.y=y/d;
        r.z=z/d;
        return r;
    }

    /**
    @date ~2016-05-20
    @brief Get norm value.
    @par Parameters
        None
    @return Norm value
    */
    GLfloat getNorm()
    {
        return sqrt(x*x+y*y+z*z);
    }

    /**
    @date ~2016-05-20
    @brief Get unit vector.
    @par Parameters
        None
    @return Vector whose length is 1
    */
    Vector3 getUnitVector()
    {
        return (*this)*(1./getNorm());
    }

    /**
    @var Vector3::x
    @date ~2016-05-20
    @brief x-axis value
    @var Vector3::y
    @date ~2016-05-20
    @brief y-axis value
    @var Vector3::z
    @date ~2016-05-20
    @brief z-axis value
    */
    float x,y,z;
};

/**
@date ~2016-05-20
@brief Camera structure
*/
struct Camera
{
    /**
    @date ~2016-05-20
    @brief Construct a Camera instance.
    @par Parameters
        None
    @return Constructed Camera instance
    */
    Camera()
    {
        position=Vector3(0,0,0);
        normal=Vector3(0,0,-1);
        xAxis=Vector3(1,0,0);
        yAxis=Vector3(0,1,0);
        azimuth=0;
        elevation=0;
        twist=0;
    }

    /**
    @var Camera::position
    @date ~2016-05-20
    @brief Position vector
    @var Camera::normal
    @date ~2016-05-20
    @brief Direction vector
    @var Camera::xAxis
    @date ~2016-05-20
    @brief x-axis vector
    @var Camera::yAxis
    @date ~2016-05-20
    @brief y-axis vector
    */
    Vector3 position,normal,xAxis,yAxis;

    /**
    @var Camera::azimuth
    @date ~2016-05-20
    @brief Rotated angle in xy-plain
    @var Camera::elevation
    @date ~2016-05-20
    @brief Rotated angle above xy-plain
    @var Camera::twist
    @date ~2016-05-20
    @brief Rotated angle around viewing direction
    */
    GLfloat azimuth,elevation,twist;
};

/**
@date ~2016-05-20
@brief 3D object face structure
*/
struct Face
{
    /**
    @date ~2016-05-20
    @brief Array for vertex numbers
    */
    vector<int> vertexNo;

    /**
    @date ~2016-05-20
    @brief Array for vertex normals
    */
    vector<Vector3> normal;
};

/**
@date 2016-05-23
@brief 3D object structure
*/
struct Object
{
    /**
    @var vertex
    @date 2016-05-23
    @brief Array for vertex
    @var normal
    @date 2016-05-23
    @brief Array for normal
    */
    vector<Vector3> vertex,normal;

    /**
    @date 2016-05-23
    @brief Array for face
    */
    vector<Face> face;

    /**
    @date 2016-05-23
    @brief Translation value
    */
    Vector3 translation;
};

/**
@date ~2016-05-20
@brief Convert raw button value to pre-defined macro.
@param [in] button Raw button value
@return Converted button value
*/
inline int glConvertButton(int button)
{
    switch(button)
    {
    case GLUT_LEFT_BUTTON:
        return KIST_MOUSE_LEFT;
    case GLUT_RIGHT_BUTTON:
        return KIST_MOUSE_RIGHT;
    default:
        return button;
    }
}

/**
@date ~2016-05-20
@brief Call glVertex3f() for given vector.
@param [in] v Target vector
@return None
*/
inline void glVertex3fv(Vector3 v)
{
    glVertex3f(v.x,v.y,v.z);
}

/**
@date 2016-05-23
@brief Call glNormal3f() for given vector.
@param [in] v Target vector
@return None
*/
inline void glNormal3fv(Vector3 v)
{
    glNormal3f(v.x,v.y,v.z);
}

/**
@date ~2016-05-20
@brief Print text to given coordinate
@param [in] x x-axis value
@param [in] y y-axis value
@param [in] z z-axis value
@param [in] string String to print
@return None
*/
void glText(GLfloat x,GLfloat y,GLfloat z,const char *string)
{
    const char *ptr;
    glRasterPos3f(x,y,z);
    for(ptr=string;*ptr!=0;ptr++)glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ptr);
}

/**
@date ~2016-05-20
@brief Call gluLookAt() for given vectors.
@param [in] eye First vector
@param [in] center Second vector
@param [in] up Third vector
@return None
*/
inline void gluLookAt(Vector3 eye,Vector3 center,Vector3 up)
{
    gluLookAt(eye.x,eye.y,eye.z,center.x,center.y,center.z,up.x,up.y,up.z);
}

/**
@date ~2016-05-20
@brief Get a dot product value within to vectors.
@param [in] a First vector
@param [in] b Second vector
@return Dot product value
*/
GLfloat dotProduct(Vector3 a,Vector3 b)
{
    return a.x*b.x+a.y*b.y+a.z*b.z;
}

/**
@date ~2016-05-20
@brief Get a cross product value within two vectors.
@param [in] a First vector
@param [in] b Second vector
@return Cross product value
*/
Vector3 crossProduct(Vector3 a,Vector3 b)
{
    Vector3 r;
    r.x=a.y*b.z-a.z*b.y;
    r.y=a.z*b.x-a.x*b.z;
    r.z=a.x*b.y-a.y*b.x;
    return r;
}

/**
@date ~2016-05-20
@brief Rotate a vector from target around axis about given angle.
@param [in] target Target vector
@param [in] axis Axis vector
@param [in] angle Rotating angle
@return Rotated vector
*/
Vector3 rotateVector(Vector3 target,Vector3 axis,float angle)
{
    Vector3 parallel=axis*(dotProduct(target,axis)/dotProduct(axis,axis));
    Vector3 vertical=target-parallel;
    Vector3 cross=crossProduct(axis,vertical);
    Vector3 result=(vertical.getUnitVector()*cos(angle)+cross.getUnitVector()*sin(angle))*vertical.getNorm()+parallel;
    return result;
}

/**
@date ~2016-05-20
@brief Determine whether the route from a through b to c is clockwise, counter-clockwise, or a line.
@param [in] a First vector
@param [in] b Second vector
@param [in] c Third vector
@param [in] camera Camera's vector
@retval 1 CCW (Counter-clockwise)
@retval -1 CW (Clockwise)
@retval 0 Line
*/
int getDirection(Vector3 a,Vector3 b,Vector3 c,Vector3 camera)
{
    GLfloat result=dotProduct(crossProduct(b-a,c-a),camera-a);
    if(result<0)return -1;
    if(result>0)return 1;
    return 0;
}

#endif // __MAIN_H__
