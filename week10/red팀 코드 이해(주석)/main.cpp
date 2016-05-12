#include"main.h"
#include <math.h>

/*
    상태
    모델 : aVertex, aFace, aNormal
    카메라 : Camera
    마우스 클릭 : buttonState (bitmap)
    키보드 입력 : isPress[]

    기능
    파일 열기 : DoLoad()
    카메라 줌 : DoMouse()
    카메라 상하좌우 이동: DoIdle(), DoMouse(), DoMouseMove(), DoMousePassiveMove(), DoSpecial()(방향키), DoKeyboard(), DoKeyboardUp
    카메라 회전 : DoMouse()

    내부
    모델 출력 : DoDisplaySample(), DoInitDisplay(), DoDisplayLightOn()
    시점, 절단 : DoDisplayMatrix()

*/


const double M_PI = 3.14;

//
GLfloat translationX,translationY,translationZ,translationStep=0.05;
//
GLfloat twist,elevation,azimuth,angleStep=2.0;
//
GLfloat scale,size,scaleStep;
//
GLfloat projectionMatrix[16];
// 클릭한 좌표, 버튼 상태
int prvX,prvY,buttonState;
// OBJ 파일명
char file[260],info[260];

map <int, bool> isPress;
int mode = -1;

        모델 출력 : DoDisplaySample()
        카메라 시점 설정 그려낼 물체 절단 : DoDisplayMatrix()
bool debug = true;
bool showGridline = true;
int plus = 0;

// 모든 상태 초기화
void initialize()
{
    //
    translationX=0;
    translationY=0;
    translationZ=0;
    elevation=0;
    azimuth=0;
        모델 출력 : DoDisplaySample()
        카메라 시점 설정 그려낼 물체 절단 : DoDisplayMatrix()
    twist=0;
    scale=1;
    scaleStep=0.05;

    Camera = CAMERA();

    prvX = -1; prvY = -1;

    mode = KIST_CAMERA_MODE;
}

// OBJ 파일 로드
void DoLoad()
{
    int i;
        모델 출력 : DoDisplaySample()
        카메라 시점 설정 그려뀀
    // 오픈할 OBJ 파일 선택
    const char *ret=tinyfd_openFileDialog(dialogTitle,NULL,1,filterPatterns,filterDescription,0);
    if(ret==NULL)return;
    for(i=(int)strlen(ret)-1;i>=0;--i)if(ret[i]=='/'||ret[i]=='\\')break;
    strcpy(file,ret+i+1);
    FILE*fp=fopen(ret,"r");
    if(fp==NULL)return;
    // 프로그램의 모델 정보 저장소 초기화
    initialize();
    aVertex.clear();
    aFace.clear();
        모델 출력 : DoDisplaySample()
        카메라 시점 설정 그려낼 물체 절단 : DoDisplayMatrix()
    size=0;
    // OBJ 파일 텍스트 끝에 도달할 때까지 반복
    while(!feof(fp))
    {
        // OBJ 파일 텍스트를 한줄씩 읽어들임
        char line[1024];
        fgets(line,1024,fp);
        // line이 벡터, 노말, 면 중 어떤 정보를 나타내는지 구별
        if(!strncmp(line,"v ",2)) // 벡터 정보
        {
            // 벡터 정보를 읽어들임
            Vector3 v;
            sscanf(line+2,"%f%f%f",&v.x,&v.y,&v.z);
            // 읽어들인 벡터 정보를 그림판 모델 저장소에 저장
        모델 출력 : DoDisplaySample()
        카멀
            aVertex.push_back(v);
            // ???
            if(v.getNorm()>size)size=v.getNorm();
        }
        else if(!strncmp(line,"vn ",3)) // 노말 정보
        {
            // 노말 정보를 읽어들임
            Vector3 v;
            sscanf(line+2,"%f%f%f",&v.x,&v.y,&v.z);
            // 노말 정보를 그림판 모델 저장소에 저장
            aNormal.push_back(-v);
        }
        else if(!strncmp(line,"f ",2)) // 면 정보
        {
            // 모델 저장소의 벡터, 노말에 접근하기 위한 index
            int t;
            // 저장할 면 A 의 노말
            Vector3 n;
            // 텅 빈 면정보 A 를 그림판 모델 저장소에 저장
            aFace.push_back(Face());
             // 읽어들인 줄에서 오른쪽으로 두칸 옮김
            char *pt=line+2;
            while(1)
            {
                // 면의 한 꼭지점 벡터 정보 읽어들임
                sscanf(pt,"%d",&t);
                // 읽어들인 꼭짓점 저장
                aFace.back().vertexNo.push_back(t);
                 // 꼭짓점이 텍스처 혹은 노말 정보를 추가로 가지는 경우 true
                if(strstr(pt,"/"))
                {
                    pt=strstr(pt,"/")+1;
                     // 꼭짓점이 노말 정보를 갖는 경우 true
                    if(strstr(pt,"/")&&strstr(pt,"/")<strstr(pt," "))
                    {
                        // a/b/c 와 같은 형식일 때, 두 번째 /로 이동
                        pt=strstr(pt,"/")+1;
                        // 꼭짓점의 노말 정보 읽어들임
                        sscanf(pt,"%d",&t);
                        // 변수n에 꼭짓점이 노말 정보 저장
                        n=aNormal[t-1];
                    }
                }
                // 꼭짓점의 노말 그림판 모델 저장소에 저장
                aFace.back().normal.push_back(n);
                // 이번 line에서 더 이상 읽어들일 꼭짓점이 없을 시 다음 line으로 넘어감
                if(strstr(pt," "))pt=strstr(pt," ")+1;
                else break;
                // 어떤 일을 하는건지 모르겠음ㅠ
                for(i='0';i<='9';++i)
                {
                    char digit[2];
                    digit[0]=i;
                    digit[1]=0;
                    if(strstr(pt,digit))break;
                }
                // 어떤 일을 하는건지 모르겠음ㅠ
                if(i>'9')break;
            }
        }
    }
    // 오픈한 OBJ 파일을 닫음
    fclose(fp);
}

// 그림판 프로그램이 놀고있을 때 자주 실행하는 우선순위가 낮은 기능?
void DoIdle() {
    //'a' 혹은 'A' 누르고 있을 동안 카메라의 위치는 카메라의 왼쪽방향으로 이동
    if(isPress['a'] || isPress['A'])
        Camera.position = Camera.position - Camera.xAxis *(float)0.05;
    //'d' 혹은 'D' 누르고
        모델 출력 : DoDisplaySample()
        카메라 시점 설정 그려뀀 있을 동안 카메라의 위치는 카메라의 오른쪽방향으로 이동
    if(isPress['d'] || isPress['D'])
        Camera.position = Camera.position + Camera.xAxis *(float)0.05;
    //'w' 혹은 'W' 누르고 있을 동안 카메라의 위치는 카메라의 위쪽 방향으로 이동
    if(isPress['w'] || isPress['W'])
        Camera.position = Camera.position + Camera.normal*(float)0.05;
    //'s' 혹은 'S' 누르고 있을 동안 카메라의 위치는 카메라의 아래 방향으로 이동
    if(isPress['s'] || isPress['S'])
        Camera.position = Camera.position - Camera.normal*(float)0.05;
    //카메라가 이동한 그림판을 다시 그림
    glutPostRedisplay();
}

// gl 초기화
void DoDisplayInit()
{
    sprintf(info,"\"%s\" - x=%.1f, y=%.1f, z=%.1f, e=%.1f, a=%.1f, t=%.1f, s=%.1f",file,translationX,translationY,translationZ,elevation,azimuth,twist,scale);
    glutSetWindowTitle(info);

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

// 시점(카메라) 처리 후 절단 처리
void DoDisplayMatrix()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
//    glOrtho(-size,size,-size,size,-size*scale,size*scale);
    glFrustum(-1, 1, -1, 1, 1, 50);
    gluLookAt(Camera.position, Camera.position + Camera.normal, Camera.yAxis);
    glScalef(scale,scale,scale);
    glTranslatef(translationX,translationY,translationZ);
    glRotatef(twist,0.0,0.0,1.0);
    glRotatef(elevation,1.0,0.0,0.0);
    glRotatef(azimuth,0.0,1.0,0.0);

}
void DoDisplayLightOn()
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
void DoDisplayGridline()
{
    if(!showGridline) return;
    glLineWidth(3);
    glBegin(GL_LINES);
    glColor3f(1,1,1); // X축 Red
    for(GLfloat i=-2.0; i<=2.0; i+=0.8) {
        for(GLfloat j=-2.0; j<=2.0; j+=0.8) {
            glVertex3f(i, j, +50.0);
            glVertex3f(i, j, -50.0);
            glVertex3f(i, j, +50.0);
            glVertex3f(i, j, -50.0);
        }
    }
    glEnd();
}
// 모델 출력
void DoDisplaySample()
{
    glGetFloatv(GL_PROJECTION_MATRIX,projectionMatrix);

    // 면 출력
    for(vector<Face>::iterator it=aFace.begin();it!=aFace.end();++it)
    {
        glBegin(GL_POLYGON);
        // 이상한 면 무시
        if(it->vertexNo.size()<3)continue;
        for(unsigned int i=0;i<it->vertexNo.size();++i)
        {
            // 노말벡터 계산 (안주어졌을 때)
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
void DoDisplayString()
{
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    drawBitmapText((char *)"z, gird(l)ine, (m)ode", -1, -0.98, Camera.position.y-1);
}
// 출력 기능
void DoDisplay()
{
    DoDisplayInit();
    // 시점, 절단 처리
    DoDisplayMatrix();
    DoDisplayLightOn();
    DoDisplayGridline();
    // 시점(카메라) 처리 후 절단 처리
    DoDisplaySample();
    DoDisplayString();
    glFlush();
}

void DoReshape(int w,int h)
{
    // TODO
}

// 비트맵으로 현재 상태 나타냄 (와와!!)
void DoMouse(int button,int state,int x,int y)
{
    button=glConvertButton(button);
    if(state==GLUT_DOWN)
    {
        switch(button)
        {
        case KIST_LEFT_BUTTON:
        case KIST_RIGHT_BUTTON:
            //상태 추가
        모델 출력 : DoDisplaySample()
        카멀
            buttonState=buttonState|button;
            //기준점 설정
            prvX=x;
            prvY=y;
            break;
        case KIST_SCROLL_UP:
            // 줌 인
            scale=scale*(1+scaleStep);
            break;
        case KIST_SCROLL_DOWN:
            // 줌 아웃
            scale=scale/(1+scaleStep);
            break;
        }
    }
    if(state==GLUT_UP)
    {
        switch(button)
        {
        case KIST_LEFT_BUTTON:
        case KIST_RIGHT_BUTTON:
            // 상태 삭제
            buttonState=buttonState&(~button);
            break;
        }
    }
    glutPostRedisplay();
}

void DoMouseMove(int x,int y)
{
    switch(buttonState)
    {
    case KIST_LEFT_BUTTON:
        // 기준점과 비교
        translationX=translationX+(x-prvX)*2.0*size/scale/WindowWidth;
        translationY=translationY-(y-prvY)*2.0*size/scale/WindowHeight;
        break;
    case KIST_RIGHT_BUTTON:
        // 기준점 극좌표와 비교
        Camera.azimuth  =Camera.azimuth  +1.0*(x-prvX)/WindowWidth *M_PI*cos(Camera.twist*M_PI/180)
                                         -1.0*(y-prvY)/WindowHeight*M_PI*sin(Camera.twist*M_PI/180);
        Camera.elevation=Camera.elevation+1.0*(x-prvX)/WindowWidth *M_PI*sin(Camera.twist*M_PI/180)
                                         +1.0*(y-prvY)/WindowHeight*M_PI*cos(Camera.twist*M_PI/180);

        // 카메라 기준점 대상 회전
        Camera.normal = rotateVector(Camera.baseNormal, Camera.baseYAxis, -Camera.azimuth);
        Camera.xAxis  = rotateVector(Camera.baseXAxis , Camera.baseYAxis, -Camera.azimuth);
        Camera.normal = rotateVector(Camera.normal    , Camera.xAxis    , -Camera.elevation);
        Camera.yAxis  = rotateVector(Camera.baseYAxis , Camera.xAxis    , -Camera.elevation);
        break;
        //카메라 자체 회전
    case KIST_BOTH_BUTTON:
        twist=twist+(atan2(-y+WindowHeight/2,x-WindowWidth/2)-atan2(-prvY+WindowHeight/2,prvX-WindowWidth/2))*180/M_PI;
        break;
    }
    prvX=x;
    prvY=y;
    glutPostRedisplay();
}
void DoMousePassiveMove(int x, int y)
{
    if(mode == KIST_SELECT_MODE) {
        prvX=x;
        prvY=y;
        return;
    }
    if(x < 10 || y < 10 || x > WindowWidth-10 || y > WindowHeight-10)
    {
        glutWarpPointer(WindowWidth/2, WindowHeight/2);
        prvX = WindowWidth / 2;
        prvY = WindowHeight / 2;
    }
    else
    {
        Camera.azimuth  =Camera.azimuth  +1.0*(x-prvX)/WindowWidth *M_PI*cos(Camera.twist*M_PI/180)
        -1.0*(y-prvY)/WindowHeight*M_PI*sin(Camera.twist*M_PI/180);
        Camera.elevation=Camera.elevation+1.0*(x-prvX)/WindowWidth *M_PI*sin(Camera.twist*M_PI/180)
        +1.0*(y-prvY)/WindowHeight*M_PI*cos(Camera.twist*M_PI/180);

        Camera.normal = rotateVector(Camera.baseNormal, Camera.baseYAxis, -Camera.azimuth);
        Camera.xAxis  = rotateVector(Camera.baseXAxis , Camera.baseYAxis, -Camera.azimuth);
        Camera.normal = rotateVector(Camera.normal    , Camera.xAxis    , -Camera.elevation);
        Camera.yAxis  = rotateVector(Camera.baseYAxis , Camera.xAxis    , -Camera.elevation);
        prvX=x;
        prvY=y;
    }

    glutPostRedisplay();
}

void DoKeyboardUp(unsigned char key, int x, int y) {
    isPress[(int)key] = false;
}

void DoKeyboard(unsigned char key,int x,int y)
{
    isPress[(int)key] = true;
    switch(key)
    {
    /*
    case 'a':
        azimuth=azimuth-angleStep;
        break;
    case 'd':
        azimuth=azimuth+angleStep;
        break;
    case 'w':
        elevation=elevation-angleStep;
        break;
    case 's':
        elevation=elevation+angleStep;
        break;
    case 'q':
        twist=twist+angleStep;
        break;
    case 'e':
        twist=twist-angleStep;
        break;
    */
    case 'x':
        translationZ=translationZ-translationStep*size;
        break;
    case 'c':
        translationZ=translationZ+translationStep*size;
        break;
    case 'z':
        initialize();
        break;
    case 'g':
        debug = !debug;
        break;
    case 'l':
        showGridline = !showGridline;
        break;
    case 'p':
        Camera.position.z += 1;
        break;
    case ' ':
        DoLoad();
        break;
    case 'm':
        if(mode == KIST_SELECT_MODE)
        {
            mode = KIST_CAMERA_MODE;
            glutSetCursor(GLUT_CURSOR_NONE); // 커서를 가린다.
        }else if(mode == KIST_CAMERA_MODE)
        {
            mode = KIST_SELECT_MODE;
            glutSetCursor(GLUT_CURSOR_INHERIT);
        }
        break;
    }
    glutPostRedisplay();
}

void DoSpecial(int key,int x,int y)
{
//    isPress[key + 256] = true;
    printf("SPECIAL %d\n", key);
    switch(key)
    {
    case GLUT_KEY_LEFT:
        translationX=translationX-translationStep*size;
        break;
    case GLUT_KEY_RIGHT:
        translationX=translationX+translationStep*size;
        break;
    case GLUT_KEY_UP:
        translationY=translationY+translationStep*size;
        break;
    case GLUT_KEY_DOWN:
        translationY=translationY-translationStep*size;
        break;
    }
    glutPostRedisplay();
}

// 프로그램 시작 지점
int main(int argc,char *argv[])
{
#if !defined(_DEBUG) && !defined(__APPLE__)
    FreeConsole();
#endif

    // 그림판 프로그램 모델 저장소로 OBJ 모델 가져오기
    DoLoad();

    // 그림판 프로그램 창 띄우기
    glutInit(&argc,argv);
    glutInitWindowSize(WindowWidth,WindowHeight);
    glutCreateWindow(NULL);

    // 그림판 프로그램 기능 추가
    glutDisplayFunc(DoDisplay); // 모델 그리기 기능
    glutReshapeFunc(DoReshape); // 모델 다시 그리는 기능
    glutMouseFunc(DoMouse); // 마우스 클릭했을 때 작동하는 기능
    glutMotionFunc(DoMouseMove); // 클릭한채로 마우스 이동했을 때 작동하는 기능
    glutPassiveMotionFunc(DoMousePassiveMove); // 마우스 이동했을 때 작동하는 기능
    glutKeyboardFunc(DoKeyboard); // 키보드 눌렀을 때 작동하는 기능
    glutKeyboardUpFunc(DoKeyboardUp); // 키보드가 눌렸다 떼어졌을 때 작동하는 기능
    glutSpecialFunc(DoSpecial); // 특수키 눌러졌을 때 작동하는 기능
    glutIdleFunc(DoIdle); // ??
#ifdef __APPLE__
    CGSetLocalEventsSuppressionInterval(0.0);
    // OS X 에서 기본 설정이 0.25라서 끊기는 현상이 발생하는 것을 방지.
    // http://goo.gl/7Zxkmp
#endif
    glutSetCursor(GLUT_CURSOR_NONE); // 커서를 가린다.

    // 그림판 프로그램 기능 시작!
    glutMainLoop();

    return EXIT_SUCCESS;
}
