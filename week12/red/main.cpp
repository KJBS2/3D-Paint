/**
@file main.cpp
@date ~2016-05-20
@brief Main source file
*/

#include"main.h"

#define _DEBUG

vector<Object> aObject;
set<int> aSelectedObject;
Camera camera,base;
GLfloat twist,angleStep=2.0;
GLfloat scale,size,scaleStep;
int prvX,prvY,buttonState,mode;
bool isLoading,isShowingGrid,isPressed[512];

/**
@date ~2016-05-20
@brief Select an object
@param direction Direction vector
@param position Position vector
@retval -1 Not any object is selected
@retval Otherwise Index for selected object
*/
int SelectObject(Vector3 direction,Vector3 position)
{
    for(unsigned int idx=0;idx<aObject.size();++idx)
    {
        for(vector<Face>::iterator it=aObject[idx].face.begin();it!=aObject[idx].face.end();++it)
        {
            if(it->vertexNo.size()<3)continue;
            Vector3 point[3];
            point[0]=aObject[idx].vertex[it->vertexNo[0]]+aObject[idx].translation;
            point[1]=aObject[idx].vertex[it->vertexNo[1]]+aObject[idx].translation;
            point[2]=aObject[idx].vertex[it->vertexNo[2]]+aObject[idx].translation;
            Vector3 normal=crossProduct(point[1]-point[0],point[2]-point[0]);
            Vector3 intersect=position+direction*dotProduct(normal,point[0]-position)/dotProduct(normal,direction);
            for(unsigned int i=1;i<it->vertexNo.size()-1;++i)
            {
                point[1]=aObject[idx].vertex[it->vertexNo[i]]+aObject[idx].translation;
                point[2]=aObject[idx].vertex[it->vertexNo[i+1]]+aObject[idx].translation;
                int cnt=0;
                for(int j=0;j<3;j++)cnt=cnt+getDirection(point[j],point[(j+1)%3],intersect,position);
                if(abs(cnt)==3)return idx;
            }
        }
    }
    return -1;
}

/**
@date ~2016-05-20
@brief Initialize variables.
@par Parameters
    None
@return None
*/
void initialize()
{
    twist=0;
    scale=1;
    scaleStep=0.05;
    buttonState=0;
    aSelectedObject.clear();
    camera=Camera();
    mode=KIST_MODE_VIEW;
}

/**
@date ~2016-05-20
@brief Load object file.
@par Parameters
    None
@return None
*/
void DoLoad()
{
    isLoading=true;
    const char *ret=tinyfd_openFileDialog(dialogTitle,NULL,1,filterPatterns,filterDescription,0);
    isLoading=false;
    if(ret==NULL)return;
    FILE*fp=fopen(ret,"r");
    if(fp==NULL)return;
    aObject.push_back(Object());
    while(!feof(fp))
    {
        char line[1024];
        fgets(line,1024,fp);
        if(!strncmp(line,"v ",2))
        {
            Vector3 v;
            sscanf(line+2,"%f%f%f",&v.x,&v.y,&v.z);
            aObject.back().vertex.push_back(v);
            if(v.getNorm()>size)size=v.getNorm();
        }
        else if(!strncmp(line,"vn ",3))
        {
            Vector3 v;
            sscanf(line+2,"%f%f%f",&v.x,&v.y,&v.z);
            aObject.back().normal.push_back(-v);
        }
        else if(!strncmp(line,"f ",2))
        {
            int t;
            Vector3 n;
            aObject.back().face.push_back(Face());
            char *pt=line+2;
            while(1)
            {
                sscanf(pt,"%d",&t);
                aObject.back().face.back().vertexNo.push_back(t);
                if(strstr(pt,"/"))
                {
                    pt=strstr(pt,"/")+1;
                    if(strstr(pt,"/")&&strstr(pt,"/")<strstr(pt," "))
                    {
                        pt=strstr(pt,"/")+1;
                        sscanf(pt,"%d",&t);
                        n=aObject.back().normal[t-1];
                    }
                }
                aObject.back().face.back().normal.push_back(n);
                if(strstr(pt," "))pt=strstr(pt," ")+1;
                else break;
                char ch;
                for(ch='0';ch<='9';++ch)
                {
                    char digit[2];
                    digit[0]=ch;
                    digit[1]=0;
                    if(strstr(pt,digit))break;
                }
                if(ch>'9')break;
            }
        }
    }
    fclose(fp);
    
    switch(mode)
    {
        case KIST_MODE_SELECT:
            glutSetCursor(GLUT_CURSOR_INHERIT);
            break;
        case KIST_MODE_VIEW:
            glutSetCursor(GLUT_CURSOR_NONE);
            break;
    }

}

/**
@date ~2016-05-20
@brief Executed in glutIdleFunc(). 연속적인 처리가 필요한 키 (방향키 and ADWS, control 등)에 대한 반응을 해준다.
@par Parameters
    None
@return None
*/
void DoIdle()
{
    if(isPressed['a']||isPressed['A'])camera.position=camera.position-camera.xAxis*0.05*3;
    if(isPressed['d']||isPressed['D'])camera.position=camera.position+camera.xAxis*0.05*3;
    if(isPressed['w']||isPressed['W'])camera.position=camera.position+camera.normal*0.05*3;
    if(isPressed['s']||isPressed['S'])camera.position=camera.position-camera.normal*0.05*3;
    if(isPressed[' '])
    {
        if(!isPressed[368])camera.position=camera.position+camera.yAxis*0.05*3;
        else camera.position=camera.position-camera.yAxis*0.05*3;
    }
    if(isPressed[256+GLUT_KEY_LEFT])camera.position=camera.position-camera.xAxis*0.05*3;
    if(isPressed[256+GLUT_KEY_RIGHT])camera.position=camera.position+camera.xAxis*0.05*3;
    if(isPressed[256+GLUT_KEY_UP])camera.position=camera.position+camera.normal*0.05*3;
    if(isPressed[256+GLUT_KEY_DOWN])camera.position=camera.position-camera.normal*0.05*3;

    
    glutPostRedisplay();
}

/**
@date ~2016-05-20
@brief Initialize display.
@par Parameters
    None
@return None
*/
void DoDisplayInit()
{
    glClearColor(0,0,0,1);

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

/**
@date ~2016-05-20
@brief Set matrix for display.
@par Parameters
    None
@return None
*/
void DoDisplayMatrix()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1,1,-1,1,1,50);
    gluLookAt(camera.position,camera.position+camera.normal,camera.yAxis);
    glScalef(scale,scale,scale);
    glRotatef(twist,0.0,0.0,1.0);
}

/**
@date ~2016-05-20
@brief Set light for display.
@par Parameters
    None
@return None
*/
void DoDisplayLight()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0,GL_AMBIENT,lightAmbient);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,lightDiffuse);
    glLightfv(GL_LIGHT0,GL_SPECULAR,lightSpecular);
    glLightfv(GL_LIGHT0,GL_POSITION,lightPosition);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);

    glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,materialAmbient);
    glMaterialfv(GL_FRONT,GL_SPECULAR,materialSpecular);
    glMaterialf(GL_FRONT,GL_SHININESS,60);
    glEnable(GL_COLOR_MATERIAL);
}

/**
@date ~2016-05-20
@brief Show grid in display.
@par Parameters
    None
@return None
*/
void DoDisplayGrid()
{
    if(!isShowingGrid)return;
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glLineWidth(3);
    glBegin(GL_LINES);
    glColor3f(0,0,1);
    for(GLfloat i=-1.6;i<=1.65;i+=0.8)
    {
        for(GLfloat j=-1.6;j<=1.65;j+=0.8)
        {
            glVertex3f(i,j,0);
            glVertex3f(i,j,-10);
        }
    }
    glEnd();
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

/**
@date ~2016-05-20
@brief Show objects in display.
@par Parameters
    None
@return None
*/
void DoDisplayObject()
{
    GLfloat projectionMatrix[16];
    glGetFloatv(GL_PROJECTION_MATRIX,projectionMatrix);
    for(unsigned int idx=0;idx<aObject.size();++idx)
    {
        if(aSelectedObject.find(idx) != aSelectedObject.end() )glColor3f(0,1,1);
        else glColor3f(1,1,1);
        for(unsigned int i=0;i<aObject[idx].face.size();++i)
        {
            Face &face=aObject[idx].face.at(i);
            if(face.vertexNo.size()<3)continue;
            glBegin(GL_POLYGON);
            for(unsigned int i=0;i<face.vertexNo.size();++i)
            {
                if(face.normal[i].x==0&&face.normal[i].y==0&&face.normal[i].z==0)
                {
                    Vector3 ba=aObject[idx].vertex[face.vertexNo[1]-1]-aObject[idx].vertex[face.vertexNo[0]-1];
                    Vector3 ca=aObject[idx].vertex[face.vertexNo[2]-1]-aObject[idx].vertex[face.vertexNo[0]-1];
                    face.normal[i]=Vector3(ca.y*ba.z-ba.y*ca.z,ca.z*ba.x-ba.z*ca.x,ca.x*ba.y-ba.x*ca.y);
                }
                glNormal3fv(Vector3(-face.normal[i].x*projectionMatrix[0]-face.normal[i].y*projectionMatrix[4]-face.normal[i].z*projectionMatrix[8],-face.normal[i].x*projectionMatrix[1]-face.normal[i].y*projectionMatrix[5]-face.normal[i].z*projectionMatrix[9],-face.normal[i].x*projectionMatrix[2]-face.normal[i].y*projectionMatrix[6]-face.normal[i].z*projectionMatrix[10]).getUnitVector());
                glVertex3fv(aObject[idx].vertex[face.vertexNo[i]-1]+aObject[idx].translation);
            }
            glEnd();
        }
    }
}

/**
@date ~2016-05-20
@brief Show laser in display.
@par Parameters
    None
@return None
*/
void DoDisplayLaser()
{
    if(mode!=KIST_MODE_SELECT)return;
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex3fv(camera.position-Vector3(0,-0.1,0)); // 실제로 손와 눈의 위치는 다르다. 눈에서 발사한다면 이론적으로 눈에서 보이지 않는다.
    glVertex3fv(camera.position+(camera.xAxis*(prvX-(GLfloat)WindowWidth/2)/(WindowWidth/2)-camera.yAxis*(prvY-(GLfloat)WindowHeight/2)/(WindowHeight/2)+camera.normal)*maxZ);
    glEnd();
}

/**
@date ~2016-05-20
@todo 언제든지 글자가 보이게 해야함 (in develop version)
@brief Show string in display.
@param [in] x x-axis value
@param [in] y y-axis value
@param [in] z z-axis value
@param [in] string String to show
@return None
*/
void DoDisplayString(GLfloat x,GLfloat y,GLfloat z,const char *string)
{
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glColor3f(1,1,1);
    glText(x,y,z,string);
}

/**
@date ~2016-05-20
@brief Executed in glDisplayFunc().
@par Parameters
    None
@return None
*/
void DoDisplay()
{
    DoDisplayInit();
    DoDisplayMatrix();
    DoDisplayLight();
    DoDisplayGrid();
    DoDisplayObject();
    DoDisplayLaser();
    DoDisplayString(-1,-0.98,camera.position.y-1,"[ASDF] Camera  [ARROW] Object  [Z] Reset  [L] Load  [G] Grid  [M] Mode");
#ifdef _DEBUG
    char info[260];
    sprintf(info,"t=%.1f, s=%.1f",twist,scale);
    DoDisplayString(-1,-0.9,camera.position.y-1,info);
#endif
    glFlush();
}

/**
@todo Realization
@date ~2016-05-20
@brief Executed in glReshapeFunc().
@param [in] w Reshaped width
@param [in] h Reshaped height
@return None
*/
void DoReshape(int w,int h)
{
    // TODO
}

/**
@date ~2016-05-20
@brief Executed in glMouseFunc().
@param [in] button Pressed mouse button
@param [in] state Mouse state
@param [in] x x-coordinate
@param [in] y y-coordinate
@return None
*/
void DoMouse(int button,int state,int x,int y)
{
#ifdef _DEBUG
    if(glutGetModifiers() & GLUT_ACTIVE_ALT)
        printf("alt\n");
    if(glutGetModifiers() & GLUT_ACTIVE_CTRL)
        printf("ctrl\n");
    if(glutGetModifiers() & GLUT_ACTIVE_SHIFT)
        printf("shift\n");
#endif

    button=glConvertButton(button);
    if(state==GLUT_DOWN)
    {
        switch(button)
        {
        case KIST_MOUSE_LEFT:
            {
            buttonState=buttonState|button;
            int selectedObject=SelectObject(camera.xAxis*(x-(GLfloat)WindowWidth/2)/(WindowWidth/2)-camera.yAxis*(y-(GLfloat)WindowHeight/2)/(WindowHeight/2)+camera.normal,camera.position);
            if(glutGetModifiers() & GLUT_ACTIVE_CTRL)
            {
                if(selectedObject == -1) break;
                if(aSelectedObject.find(selectedObject) != aSelectedObject.end())
                    aSelectedObject.erase(selectedObject);
                else
                    aSelectedObject.insert(selectedObject);
            }
            else
            {
                if(aSelectedObject.find(selectedObject) == aSelectedObject.end())
                {
                    aSelectedObject.clear();
                    if(selectedObject != -1)
                        aSelectedObject.insert(selectedObject);
                }
            }
                
            }
            break;
        case KIST_MOUSE_RIGHT:
            buttonState=buttonState|button;
            break;
        case KIST_MOUSE_SCROLLUP:
            scale=scale*(1+scaleStep);
            break;
        case KIST_MOUSE_SCROLLDOWN:
            scale=scale/(1+scaleStep);
            break;
        }
    }
    if(state==GLUT_UP)
    {
        switch(button)
        {
        case KIST_MOUSE_LEFT:
        case KIST_MOUSE_RIGHT:
            buttonState=buttonState&(~button);
            break;
        }
    }
    prvX=x;
    prvY=y;
    glutPostRedisplay();
}

/**
@date ~2016-05-20
@brief Executed in glMotionFunc().
@param [in] x x-coordinate
@param [in] y y-coordinate
@return None
*/
void DoMotion(int x,int y)
{
    switch(buttonState)
    {
    case KIST_MOUSE_LEFT:
        for(set<int>::iterator it=aSelectedObject.begin(); it!=aSelectedObject.end(); it++) {
            int selectedObject = *it;
            aObject[selectedObject].translation=aObject[selectedObject].translation+(camera.xAxis*(GLfloat)(x-prvX)/(WindowWidth/2)*2-camera.yAxis*(GLfloat)(y-prvY)/(WindowHeight/2)*2)/scale;
        }
        break;
    case KIST_MOUSE_RIGHT:
        camera.azimuth=camera.azimuth+1.0*(x-prvX)/WindowWidth*M_PI*cos(camera.twist*M_PI/180)-1.0*(y-prvY)/WindowHeight*M_PI*sin(camera.twist*M_PI/180);
        camera.elevation=camera.elevation+1.0*(x-prvX)/WindowWidth*M_PI*sin(camera.twist*M_PI/180)+1.0*(y-prvY)/WindowHeight*M_PI*cos(camera.twist*M_PI/180);
        camera.normal=rotateVector(base.normal,base.yAxis,-camera.azimuth);
        camera.xAxis=rotateVector(base.xAxis,base.yAxis,-camera.azimuth);
        camera.normal=rotateVector(camera.normal,camera.xAxis,-camera.elevation);
        camera.yAxis=rotateVector(base.yAxis,camera.xAxis,-camera.elevation);
        break;
    case KIST_MOUSE_BOTH:
        twist=twist+(atan2(-y+WindowHeight/2,x-WindowWidth/2)-atan2(-prvY+WindowHeight/2,prvX-WindowWidth/2))*180/M_PI;
        break;
    }
    prvX=x;
    prvY=y;
    glutPostRedisplay();
}

/**
@date ~2016-05-20
@brief Executed in glMousePassiveMotionFunc().
@param [in] x x-coordinate
@param [in] y y-coordinate
@return None
*/
void DoMousePassiveMotion(int x,int y)
{
    if(isLoading)return;
    if(mode==KIST_MODE_SELECT)
    {
        prvX=x;
        prvY=y;
        return;
    }
    if(x!=prvX||y!=prvY)
    {
        camera.azimuth=camera.azimuth+1.0*(x-prvX)/WindowWidth*M_PI*cos(camera.twist*M_PI/180)-1.0*(y-prvY)/WindowHeight*M_PI*sin(camera.twist*M_PI/180);
        camera.elevation=camera.elevation+1.0*(x-prvX)/WindowWidth*M_PI*sin(camera.twist*M_PI/180)+1.0*(y-prvY)/WindowHeight*M_PI*cos(camera.twist*M_PI/180);
        camera.normal=rotateVector(base.normal,base.yAxis,-camera.azimuth);
        camera.xAxis=rotateVector(base.xAxis,base.yAxis,-camera.azimuth);
        camera.normal=rotateVector(camera.normal,camera.xAxis,-camera.elevation);
        camera.yAxis=rotateVector(base.yAxis,camera.xAxis,-camera.elevation);
        prvX=x;
        prvY=y;
        glutWarpPointer(WindowWidth/2,WindowHeight/2);
        prvX=WindowWidth/2;
        prvY=WindowHeight/2;
    }
    glutPostRedisplay();
}

/**
@date ~2016-05-20
@todo Load하는 경우 마우스커서가 보이게 해야함.
@todo Group Object를 만들때에 vector<Vector3> normal에 대해 생각하지않음.
@brief Executed in glKeyboardFunc().
@param [in] key Rressed key code
@param [in] x x-coordinate of mouse pointer
@param [in] y y-coordinate of mouse pointer
@return None
*/
void DoKeyboard(unsigned char key,int x,int y)
{
#ifdef _DEBUG
    printf("[KEY] %d %d %d\n",key,x,y);
#endif

    isPressed[key]=true;
    switch(key)
    {
    case 'z':
    case 'Z':
        initialize();
        break;
    case 'd':
    case 'D':
        isShowingGrid=!isShowingGrid;
        break;
    case 'l':
    case 'L':
        if(mode == KIST_MODE_SELECT)
        {
            glutSetCursor(GLUT_CURSOR_INHERIT);
            DoLoad();
        }
        break;
    case 'g':
    case 'G':
        {
        if(mode == KIST_MODE_SELECT)
        {
            if(aSelectedObject.size() == 0) break;
            for(set<int>::iterator it=aSelectedObject.begin(); it!=aSelectedObject.end(); it++)
            {
                int idx = *it;
                for(unsigned int i=0;i<aObject[idx].vertex.size();++i)
                    aObject[idx].vertex[i] = aObject[idx].vertex[i] + aObject[idx].translation;
                aObject[idx].translation = Vector3(0,0,0);
            }
            int beginIdx = *(aSelectedObject.begin());
            for(set<int>::iterator it=aSelectedObject.begin(); it!=aSelectedObject.end(); it++)
            {
                int idx = *it;
                if(idx == beginIdx) continue;
                
                int nowVertexCount = (int)aObject[beginIdx].vertex.size();
                for(unsigned int i=0;i<aObject[idx].vertex.size();++i)
                    aObject[beginIdx].vertex.push_back(aObject[idx].vertex[i]);
                for(unsigned int i=0;i<aObject[idx].face.size();++i)
                {
                    Face &nowFace=aObject[idx].face.at(i);
                    aObject[beginIdx].face.push_back(Face());
                    for(unsigned int i=0;i<nowFace.vertexNo.size();++i)
                    {
                        aObject[beginIdx].face.back().vertexNo.push_back(nowFace.vertexNo[i]+nowVertexCount);
                        aObject[beginIdx].face.back().normal.push_back(nowFace.normal[i]);
                    }
                    
                }
            }
            
            set<int>::iterator it=aSelectedObject.end();
            do
            {
                it--;
                int idx = *it;
                if(idx == beginIdx) break;
                aObject.erase(aObject.begin()+idx);
            }while(it!=aSelectedObject.begin());
            
        }
        }
        break;
    case 'm':
    case 'M':
    case 27: // ESC
        switch(mode)
        {
        case KIST_MODE_SELECT:
            mode=KIST_MODE_VIEW;
            glutSetCursor(GLUT_CURSOR_NONE);
            break;
        case KIST_MODE_VIEW:
            mode=KIST_MODE_SELECT;
            glutSetCursor(GLUT_CURSOR_INHERIT);
            break;
        }
        break;
    }
    glutPostRedisplay();
}

/**
@date ~2016-05-20
@brief Executed in glKeyboardUpFunc().
@param [in] key Released key code
@param [in] x x-coordinate of mouse pointer
@param [in] y y-coordinate of mouse pointer
@return None
*/
void DoKeyboardUp(unsigned char key,int x,int y)
{
    isPressed[key]=false;
}

/**
@date ~2016-05-20
@brief Executed in glSpecialUpFunc().
@param [in] key Pressed key code
@param [in] x x-coordinate of mouse pointer
@param [in] y y-coordinate of mouse pointer
@return None
*/
void DoSpecial(int key,int x,int y)
{
#ifdef _DEBUG
    printf("[SPECIAL] %d\n",256+key);
#endif
    isPressed[256+key]=true;
}

/**
@date ~2016-05-20
@brief Executed in glSpecialFunc().
@param [in] key Released key code
@param [in] x x-coordinate of mouse pointer
@param [in] y y-coordinate of mouse pointer
@return None
*/
void DoSpecialUp(int key,int x,int y)
{
    isPressed[256+key]=false;
}

/**
@date ~2016-05-20
@brief Start program and call OpenGL functions.
@param [in] argc Argument count
@param [in] argv Argument vector
@retval 0 Program exited successfully
@retval Otherwise Program exited with an error
*/
int main(int argc,char *argv[])
{
#if !defined(_DEBUG)&&!defined(__APPLE__)
    FreeConsole();
#endif
#ifdef __APPLE__
    CGSetLocalEventsSuppressionInterval(0.0); // http://goo.gl/7Zxkmp
#endif
    initialize();
    glutInit(&argc,argv);
    glutInitWindowSize(WindowWidth,WindowHeight);
    glutCreateWindow("3D-Paint");
    glutSetCursor(GLUT_CURSOR_NONE);
    glutDisplayFunc(DoDisplay);
    glutReshapeFunc(DoReshape);
    glutMouseFunc(DoMouse);
    glutMotionFunc(DoMotion);
    glutPassiveMotionFunc(DoMousePassiveMotion);
    glutKeyboardFunc(DoKeyboard);
    glutKeyboardUpFunc(DoKeyboardUp);
    glutSpecialFunc(DoSpecial);
    glutSpecialUpFunc(DoSpecialUp);
    glutIdleFunc(DoIdle);
    glutMainLoop();
    return EXIT_SUCCESS;
}
