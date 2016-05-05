#include "childwindow.h"
#include "header.h"
#include<windows.h>
#include<stdio.h>
#include "mesh.h"
#include "parser.h"

using namespace std;
ChildWindow::ChildWindow(OPENFILENAME _OFN)
{
    OFN=_OFN;

    parser.setMesh(&mesh);
    parser.setPath(OFN.lpstrFile);
    parser.parse();

    set_window();
    //파싱해서 정보들을 저장하는 함수 호출 해야되는 부분
}


LRESULT CALLBACK ChildWndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
    static PAINTSTRUCT ps;

    switch(iMessage)
    {
    case WM_CREATE:
        window_main.get_child_window()->get_mesh()->display();
        BeginPaint(hWnd,&ps);
        EndPaint(hWnd,&ps);
    }
    return (DefWindowProc(hWnd,iMessage,wParam,lParam));
}
OPENFILENAME* ChildWindow::get_openfilename()
{
    return &OFN;
}
Mesh* ChildWindow::get_mesh()
{
    return &mesh;
}
void ChildWindow::set_window()
{
    win.cbClsExtra=0;
    win.cbWndExtra=0;
    win.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
    win.hCursor=LoadCursor(NULL,IDC_ARROW);
    win.hIcon=LoadIcon(NULL,IDI_APPLICATION);
    win.hInstance=g_hInst;
    win.lpfnWndProc=(WNDPROC)ChildWndProc;
    win.lpszMenuName=MAKEINTRESOURCE(NULL);
    win.lpszClassName="Child";
    win.style=CS_HREDRAW|CS_VREDRAW;
    RegisterClass(&win);
    CreateWindowEx(0,"Child","Child",WS_CHILD|WS_VISIBLE|WS_SYSMENU|WS_CLIPCHILDREN ,2,30,980,650,window_main.get_handle(),(HMENU)NULL,g_hInst,NULL);
}
