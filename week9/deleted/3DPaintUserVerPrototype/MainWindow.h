#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Window.h"
#include "header.h"

class MainWindow : public Window
{
    public:
        MainWindow(WindowsManager *pWindowsManager);
        virtual ~MainWindow();

        //Override
        LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);

    protected:

    private:
        WindowsManager *m_pWindowsManager;
};

#endif // MAINWINDOW_H
