#include "MainWindow.h"

MainWindow::MainWindow(WindowsManager *pWindowsManager)
{
    m_lpszClass = "MainWindow";

    // Set WindowsManager because MainWindow have to access Other window classes
    // i.e. MainWindow is a ParentWindow
    m_pWindowsManager = pWindowsManager;
}

MainWindow::~MainWindow()
{
    //dtor
}

LRESULT MainWindow::WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    switch(iMessage){
    case WM_CREATE:
        //m_pWindowsManager->GetpToolBarWindow()->SethWnd(
        //                        CreateWindow())
    }
}
