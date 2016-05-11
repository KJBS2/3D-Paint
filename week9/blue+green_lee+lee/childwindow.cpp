#include "childwindow.h"
#include "header.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>

using namespace std;

bool bEnableOpenGL = false;
bool is_button_down = false;
POINT temp;
int *pX,*pY;
// child window message call back function
LRESULT CALLBACK ChildWndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{

    PAINTSTRUCT ps;

    if(bEnableOpenGL)window_main.get_child_window()->getMesh()->CheckKeyboard();
    switch(iMessage)
    {
        case WM_LBUTTONDOWN:
            is_button_down=true;
            return 0;
        case WM_LBUTTONUP:
            is_button_down=false;
            pX=&(window_main.get_child_window()->prvX);
            pY=&(window_main.get_child_window()->prvY);
            *pX=*pY=-1;
            return 0;
        case WM_PAINT:
            if(bEnableOpenGL){
            // Get mesh data
                Mesh *mesh = window_main.get_child_window()->getMesh();
            // Display mesh data
                mesh->DoDisplay();
            }
            return 0;
        case WM_MOUSEMOVE:
            if(is_button_down)
            {
                pX=&(window_main.get_child_window()->prvX);
                pY=&(window_main.get_child_window()->prvY);
                switch(window_main.get_button_stat())
                {
                case BUTTON_ROTATE_CAMERA:
                    GetCursorPos(&temp);
                    if(*pX==-1 && *pY==-1) *pX=temp.x,*pY=temp.y;
                    window_main.get_child_window()->getMesh()->DoRotateCamera(temp.x,temp.y,*pX,*pY);
                    *pX=temp.x;
                    *pY=temp.y;
                    break;
                case BUTTON_MOVE_CAMERA:
                    printf("@");
                    break;
                }
            }
            return 0;


    }

    return (DefWindowProc(hWnd,iMessage,wParam,lParam));
}



ChildWindow::ChildWindow(OPENFILENAME _OFN)
{
    OFN=_OFN;
    prvX=prvY=-1;
    set_window();

    // Set .obj file path in parser object
    parser.setPath(OFN.lpstrFile);
    // Set mesh
    parser.setMesh(&mesh);
    // Parse .obj file to mesh data format
    parser.parse();
    // Device Context, Render Context Initialization
    glContext.init(hwnd);

    // Initialize OpenGL settings
    initGL();

    // From here, you can use openGL.
    bEnableOpenGL = true;
}



void ChildWindow::set_window()
{
    win.cbClsExtra=0;
    win.cbWndExtra=0;
    win.hbrBackground=(HBRUSH)GetStockObject(BLACK_BRUSH);
    win.hCursor=LoadCursor(NULL,IDC_ARROW);
    win.hIcon=LoadIcon(NULL,IDI_APPLICATION);
    win.hInstance=g_hInst;
    win.lpfnWndProc=(WNDPROC)ChildWndProc;
    win.lpszMenuName=MAKEINTRESOURCE(NULL);
    win.lpszClassName="Child";
    win.style=CS_HREDRAW|CS_VREDRAW;
    RegisterClass(&win);


    hwnd = CreateWindowEx(0,"Child","Child",WS_CHILD|WS_VISIBLE|WS_SYSMENU|WS_CLIPCHILDREN ,2,30,980,650,window_main.get_handle(),(HMENU)NULL,g_hInst,NULL);
}

OPENFILENAME* ChildWindow::get_openfilename(){
    return &OFN;
}
Mesh* ChildWindow::getMesh(){
    return &mesh;
}
Parser* ChildWindow::getParser(){
    return &parser;
}
GLContext* ChildWindow::getGLContext(){
    return &glContext;
}
HWND ChildWindow::getHWND(){
    return hwnd;
}

void ChildWindow::initGL(){
    glClearColor(1.0, 1.0, 1.0, 1.0);
}

