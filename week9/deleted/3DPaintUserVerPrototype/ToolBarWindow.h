#ifndef TOOLBARWINDOW_H
#define TOOLBARWINDOW_H

#include "Window.h"
#include "header.h"


class ToolBarWindow : public Window
{
    public:
        ToolBarWindow();
        virtual ~ToolBarWindow();

        //Override
        LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);
    protected:

    private:
};

#endif // TOOLBARWINDOW_H
