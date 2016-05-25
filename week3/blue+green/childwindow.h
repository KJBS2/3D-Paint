#include<windows.h>
#include"parser.h"
#include"mesh.h"
#ifndef _childwindow_h_
#define _childwindow_h_
class ChildWindow
{
public:
    ChildWindow()=delete; // �⺻ ������ ����.
    ChildWindow(OPENFILENAME _OFN);
    OPENFILENAME* get_openfilename(); // getter
    void set_window();
    Mesh* get_mesh();
    // �Ľ��ϴ� �Լ��߰�.

private:
    Parser parser;
    Mesh mesh;
    OPENFILENAME OFN; // childwindow�� �ε��ؾ��ϴ� ������ ������ ����ִ� ����ü.
    HINSTANCE hinst; // ���μ����� �ڵ�
    WNDCLASS win; // window ����ü
    HWND hwnd;    // window�� �����Ǹ�, �ڵ��� ���⿡ �����Ѵ�.


    /*
        obj���� �ҷ��� �������� �����ؾ���..
                                */
};
#endif
