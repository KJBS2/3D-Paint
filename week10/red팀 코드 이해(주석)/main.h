/*
    그림판의 모델 정보 저장소 :
        점 저장     : vector<Vector3> aVertex
        노말 저장   : vector<Vector3> aNormal
        면 저장     : vector<Face> aFace

        카메라 저장 : CAMERA Camera
*/
#ifndef __MAIN_H__
#define __MAIN_H__

#include<vector>
using namespace std;

#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<map>
#include"tinyfiledialogs.h"

#ifdef __APPLE__
#include <ApplicationServices/ApplicationServices.h>
#include<GLUT/glut.h>
#else
#include<GL/glut.h>
#endif

using namespace std;

#define KIST_CAMERA_MODE 0
#define KIST_SELECT_MODE 1

#define KIST_LEFT_BUTTON    0x0010
#define KIST_RIGHT_BUTTON   0x0020
#define KIST_BOTH_BUTTON    0x0030
#define KIST_SCROLL_UP      0x0003
#define KIST_SCROLL_DOWN    0x0004

const int WindowWidth=600;
const int WindowHeight=600;

const char *dialogTitle="Load";
const char *filterPatterns[]={"*.obj"};
const char *filterDescription="Object files";

struct Vector3
{
    Vector3(float x=0,float y=0,float z=0)
    {
        this->x=x;
        this->y=y;
        this->z=z;
    }
    static int test() {
        return 1;
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
    Vector3 operator-(Vector3 v)
    {
        Vector3 r;
        r.x=x-v.x;
        r.y=y-v.y;
        r.z=z-v.z;
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
    Vector3 UnitVector()
    {
        return (*this) * (1. / getNorm());
    }
    void PrintValue() {
        printf("[%lf %lf %lf]\n", x, y, z);
    }
    float x,y,z;
};

// 카메라 정보 정의
struct CAMERA{
    Vector3 position, normal/*카메라의 위쪽 방향*/, yAxis, xAxis;
    // 카메라 회전시 기준?
    Vector3 baseNormal, baseYAxis, baseXAxis;
    // 카메라의 위치 극촤표
    float azimuth/* 카메라 위치 방위각 */, elevation/* 카메라 위치 고도각 */, twist /* p.300에 상향 벡터라고 써있지만, 정확히 뭔지 모르겠음. */;
    CAMERA() {
        position = Vector3(0,0,0); // 카메라 초기 위치는 원점
        baseNormal = normal = Vector3(0,0,-1); // 카메라 노말은 -z방향
        baseXAxis = xAxis = Vector3(1,0,0); // 카메라 초기 x축은 전역좌표계의 x축과 동일
        baseYAxis = yAxis = Vector3(0,1,0); // 카메라 초기 y축은 전역좌표계의 y축과 동일
        azimuth = elevation = twist = 0; // 카메라의 초기 극좌표는 원점
    }
}Camera; // 카메라 정보 저장

// 면 정보 정의
struct Face
{
    vector<int> vertexNo; // 면을 구성하는 꼭짓점들의 index 저장
    vector<Vector3> normal; // 면을 구성하는 꼭짓점들의 노말 저장
};

vector<Vector3> aVertex,aNormal;
vector<Face> aFace;

// 그림판에 벡터를 추가
inline void glVertex3fv(Vector3 &v)
{
    glVertex3f(v.x,v.y,v.z);
}

// 마우스 버튼 클릭시 발생하는 GLUT 상수정보를 KIST_..._BUTTON 상수로 변환
inline int glConvertButton(int button)
{
    switch(button)
    {
    case GLUT_LEFT_BUTTON:
        return KIST_LEFT_BUTTON;
    case GLUT_RIGHT_BUTTON:
        return KIST_RIGHT_BUTTON;
    default:
        return button;
    }
}

// 그림판에 텍스트 출력
void drawBitmapText(char *string,float x,float y,float z)
{
    char *c;
    // 그림판에서 텍스트를 출력할 위치로 이동
    glRasterPos3f(x, y, z);

    // null character가 나올때까지 문자열의 글자 하나하나씩 출력
    for (c=string; *c != 0; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c); // 헬베티카 서체 18 포인트로 한글자 출력
        /*
         GLUT_BITMAP_TIMES_ROMAN_10
         GLUT_BITMAP_TIMES_ROMAN_24
         GLUT_BITMAP_HELVETICA_10
         GLUT_BITMAP_HELVETICA_12
         GLUT_BITMAP_HELVETICA_18
        */
    }
}
// 카메라 시점 설정
void gluLookAt(Vector3 eye/*카메라 위치*/, Vector3 center/*카메라가 바라보는 위치 = 시점*/, Vector3 up/*카메라 위쪽 방향 벡터*/) {
    gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);
}

// 두 베터의 내적 계산
float InnerProduct(Vector3 A, Vector3 B) {
    return A.x * B.x + A.y * B.y + A.z * B.z;
}

// 두 벡터의 외적 계산
Vector3 CrossProduct(Vector3 A, Vector3 B) {
    Vector3 r;
    r.x = A.y * B.z - A.z * B.y;
    r.y = A.z * B.x - A.x * B.z;
    r.z = A.x * B.y - A.y * B.x;
    return r;
}

// 회전축을 중심으로 회전한 점 위치 계산
Vector3 rotateVector(Vector3 target/*초기 점 위치*/, Vector3 axis/*회전축*/, float theta/* 회전 각 */) {
    Vector3 parallel = axis * (InnerProduct(target, axis) / InnerProduct(axis, axis));
    Vector3 vertical = target - parallel;
    Vector3 cross = CrossProduct(axis, vertical);

    Vector3 result = (vertical.UnitVector() * cos(theta) + cross.UnitVector() * sin(theta)) * vertical.getNorm() + parallel;
    return result;
}

#endif // __MAIN_H__









