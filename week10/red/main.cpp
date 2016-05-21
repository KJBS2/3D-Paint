/**
@file main.cpp
@date ~2016-05-20
@brief Main source file
*/

#include"main.h"

vector<Vector3> aVertex,aNormal;
vector<Face> aFace;
vector<Vector3> aTranslation;
vector<Index> aIndex;

Camera camera,base;
GLfloat translationX,translationY,translationZ,translationStep=0.05;
GLfloat twist,angleStep=2.0;
GLfloat scale,size,scaleStep;
GLfloat projectionMatrix[16];
int prvX,prvY,buttonState,selectState;
char file[260],info[260];

bool isPressed[512];
int mode = -1;

bool showGrid = true;


/**
@todo
*/
int SelectModel(Vector3 direction, Vector3 position)
{
    for(vector<Index>::iterator index = aIndex.begin(); index != aIndex.end(); ++index)
    {
        int count = (int)(index - aIndex.begin());
        vector<Face>::iterator startIndex = aFace.begin() + index->start;
        vector<Face>::iterator   endIndex = aFace.begin() + index->end + 1;

        for(vector<Face>::iterator it=startIndex; it!=endIndex; ++it)
        {
            if(it->vertexNo.size()<3)continue;
            Vector3 point[3];
            point[0] = aVertex[it->vertexNo[0]] + aTranslation[count];
            point[1] = aVertex[it->vertexNo[1]] + aTranslation[count];
            point[2] = aVertex[it->vertexNo[2]] + aTranslation[count];
            Vector3 normal = crossProduct(point[1]-point[0], point[2]-point[0]);
            float t = dotProduct(normal, point[0] - position)/dotProduct(normal, direction);
            Vector3 intersect = position + direction * t;

            for(unsigned int i=1;i<it->vertexNo.size()-1;++i)
            {
                point[1] = aVertex[it->vertexNo[i]]   + aTranslation[count];
                point[2] = aVertex[it->vertexNo[i+1]] + aTranslation[count];
                int cnt = 0;
                for(int k=0; k<3; k++) {
                    int nowDir = getDirection(point[k], point[(k+1)%3], intersect, position);
                    cnt += nowDir;
                }
                if(cnt == -3 || cnt == 3) {
                    return (int)(index - aIndex.begin());
                }
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
    translationX=0;
    translationY=0;
    translationZ=0;
    twist=0;
    scale=1;
    scaleStep=0.05;
    buttonState=0;
    selectState=-1;
    camera=Camera();
    mode=KIST_MODE_VIEW;
}

/**
@todo
@date ~2016-05-20
@brief Load object file.
@par Parameters
    None
@return None
*/
void DoLoad()
{
    Index nowIndex;
    nowIndex.start = (int)aFace.size();
    int baseVertexIndex = (int)(aVertex.size());
    int baseNormalIndex = (int)(aNormal.size());

    int i;
    const char *ret=tinyfd_openFileDialog(dialogTitle,NULL,1,filterPatterns,filterDescription,0);
    if(ret==NULL)return;
    for(i=(int)strlen(ret)-1;i>=0;--i)if(ret[i]=='/'||ret[i]=='\\')break;
    strcpy(file,ret+i+1);
    FILE*fp=fopen(ret,"r");
    if(fp==NULL)return;
//    aVertex.clear();
//    aFace.clear();
    size=0;
    while(!feof(fp))
    {
        char line[1024];
        fgets(line,1024,fp);
        if(!strncmp(line,"v ",2))
        {
            Vector3 v;
            sscanf(line+2,"%f%f%f",&v.x,&v.y,&v.z);
            aVertex.push_back(v);
            if(v.getNorm()>size)size=v.getNorm();
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
            char *pt=line+2;
            while(1)
            {
                sscanf(pt,"%d",&t);
                aFace.back().vertexNo.push_back(t + baseVertexIndex);
                if(strstr(pt,"/"))
                {
                    pt=strstr(pt,"/")+1;
                    if(strstr(pt,"/")&&strstr(pt,"/")<strstr(pt," "))
                    {
                        pt=strstr(pt,"/")+1;
                        sscanf(pt,"%d",&t);
                        n=aNormal[t-1 + baseNormalIndex];
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
                if(i>'9')break;
            }
        }
    }
    nowIndex.end = (int)aFace.size() - 1;
    aIndex.push_back(nowIndex);
    aTranslation.push_back(Vector3(0,0,0));
    fclose(fp);
}

/**
@date ~2016-05-20
@brief Executed in glutIdleFunc().
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
    if(isPressed[256+GLUT_KEY_LEFT])translationX=translationX-translationStep*size;
    if(isPressed[256+GLUT_KEY_RIGHT])translationX=translationX+translationStep*size;
    if(isPressed[256+GLUT_KEY_UP])translationY=translationY+translationStep*size;
    if(isPressed[256+GLUT_KEY_DOWN])translationY=translationY-translationStep*size;
    if(isPressed[','])translationZ=translationZ-translationStep*size;
    if(isPressed['.'])translationZ=translationZ+translationStep*size;
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
    glTranslatef(translationX,translationY,translationZ);
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
    GLfloat lightAmbient[]={0.3,0.3,0.3,1};
    GLfloat lightDiffuse[]={0.6,0.6,0.6,1};
    GLfloat lightSpecular[]={1,1,1,1};
    GLfloat lightPosition[]={0,0,-size,0};
    GLfloat materialAmbient[]={0.4,0.4,0.4,1};
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
    if(!showGrid)return;
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
@todo
*/
void DoDisplayObject()
{
    glGetFloatv(GL_PROJECTION_MATRIX,projectionMatrix);
    for(vector<Index>::iterator index = aIndex.begin(); index != aIndex.end(); ++index)
    {
        int count = (int)(index - aIndex.begin());
        vector<Face>::iterator startIndex = aFace.begin() + index->start;
        vector<Face>::iterator   endIndex = aFace.begin() + index->end + 1;

        if(count==selectState)glColor3f(0,1,1);
        else glColor3f(1,1,1);

        for(vector<Face>::iterator it=startIndex; it!=endIndex; ++it)
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
                Vector3 change = aVertex[it->vertexNo[i]-1] + aTranslation[count];
                glVertex3fv(change);
            }
            glEnd();
        }
    }
}

/**
@todo
*/
void DoDisplayLaser()
{
    if(mode!=KIST_MODE_SELECT)return;
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);

    float newX = (prvX - (float)WindowWidth /2) / (WindowWidth /2);
    float newY = (prvY - (float)WindowHeight/2) / (WindowHeight/2);


    glLineWidth(3);
    glBegin(GL_LINES);
    Vector3 startPoint = camera.position;
    startPoint.y = startPoint.y - 0.1; // 실제로 손와 눈의 위치는 다르다. 눈에서 발사한다면 이론적으로 눈에서 보이지 않는다.
    glColor3f(0,1,0);
    Vector3 endPoint = camera.position + camera.xAxis * newX * 50 - camera.yAxis * newY * 50 + camera.normal * 50;
    glVertex3fv(startPoint);
    glVertex3fv(endPoint);
    glEnd();

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

/**
@date ~2016-05-20
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
    sprintf(info,"x=%.1f, y=%.1f, z=%.1f, t=%.1f, s=%.1f",translationX,translationY,translationZ,twist,scale);
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
@todo
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
    button=glConvertButton(button);
    if(state==GLUT_DOWN)
    {
        switch(button)
        {
        case KIST_MOUSE_LEFT:
            {

            buttonState=buttonState|button;
            prvX=x;
            prvY=y;
            float newX = (prvX - (float)WindowWidth /2) / (WindowWidth /2);
            float newY = (prvY - (float)WindowHeight/2) / (WindowHeight/2);
            Vector3 direction = camera.xAxis * newX - camera.yAxis * newY + camera.normal;
#ifdef _DEBUG
            printf("[%d %d -> %d]\n", x, y, selectState);
#endif
            selectState = SelectModel(direction, camera.position);
            glutPostRedisplay();
            break;
            }
        case KIST_MOUSE_RIGHT:
            buttonState=buttonState|button;
            prvX=x;
            prvY=y;
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
            buttonState=buttonState&(~button);
            break;
        case KIST_MOUSE_RIGHT:
            buttonState=buttonState&(~button);
            break;
        }
    }
    glutPostRedisplay();
}

/**
@todo
*/
void DoMouseMove(int x,int y)
{
    switch(buttonState)
    {
        case KIST_MOUSE_LEFT:
        {
            if(selectState == -1) break;

            float newX = (float)(x-prvX) / (WindowWidth /2)*2;
            float newY = (float)(y-prvY) / (WindowHeight/2)*2;

            Vector3 nowTranslation = Vector3(0,0,0);
            nowTranslation = nowTranslation + camera.xAxis * newX;
            nowTranslation = nowTranslation - camera.yAxis * newY;

            aTranslation[selectState] = aTranslation[selectState] + nowTranslation;
            break;
        }
    case KIST_MOUSE_RIGHT:
        camera.azimuth  =camera.azimuth  +1.0*(x-prvX)/WindowWidth *M_PI*cos(camera.twist*M_PI/180)
                                         -1.0*(y-prvY)/WindowHeight*M_PI*sin(camera.twist*M_PI/180);
        camera.elevation=camera.elevation+1.0*(x-prvX)/WindowWidth *M_PI*sin(camera.twist*M_PI/180)
                                         +1.0*(y-prvY)/WindowHeight*M_PI*cos(camera.twist*M_PI/180);
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
@todo
*/
void DoMousePassiveMove(int x,int y)
{
    if(mode==KIST_MODE_SELECT)
    {
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
        camera.azimuth  =camera.azimuth  +1.0*(x-prvX)/WindowWidth *M_PI*cos(camera.twist*M_PI/180)
        -1.0*(y-prvY)/WindowHeight*M_PI*sin(camera.twist*M_PI/180);
        camera.elevation=camera.elevation+1.0*(x-prvX)/WindowWidth *M_PI*sin(camera.twist*M_PI/180)
        +1.0*(y-prvY)/WindowHeight*M_PI*cos(camera.twist*M_PI/180);

        camera.normal = rotateVector(base.normal, base.yAxis, -camera.azimuth);
        camera.xAxis  = rotateVector(base.xAxis , base.yAxis, -camera.azimuth);
        camera.normal = rotateVector(camera.normal    , camera.xAxis    , -camera.elevation);
        camera.yAxis  = rotateVector(base.yAxis , camera.xAxis    , -camera.elevation);
        prvX=x;
        prvY=y;
    }

    glutPostRedisplay();
}

/**
@date ~2016-05-20
@brief Executed in glKeyboardFunc().
@param [in] key Rressed key code
@param [in] x ?
@param [in] y ?
@return None
*/
void DoKeyboard(unsigned char key,int x,int y)
{
#ifdef _DEBUG
    printf("[KEY] %d\n",(int)key);
#endif
    isPressed[key]=true;
    switch(key)
    {
    case 'z':
        initialize();
        break;
    case 'g':
        showGrid=!showGrid;
        break;
    case 'l':
        DoLoad();
        break;
    case 'm':
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
@param [in] x ?
@param [in] y ?
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
@param [in] x ?
@param [in] y ?
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
@param [in] x ?
@param [in] y ?
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
    glutMotionFunc(DoMouseMove);
    glutPassiveMotionFunc(DoMousePassiveMove);
    glutKeyboardFunc(DoKeyboard);
    glutKeyboardUpFunc(DoKeyboardUp);
    glutSpecialFunc(DoSpecial);
    glutSpecialUpFunc(DoSpecialUp);
    glutIdleFunc(DoIdle);

    glutMainLoop();

    return EXIT_SUCCESS;
}
