#ifndef DRAWINGBOARDWINDOW_H
#define DRAWINGBOARDWINDOW_H

#include "Window.h"
#include "header.h"


class DrawingBoardWindow : public Window
{
    public:
        DrawingBoardWindow();
        virtual ~DrawingBoardWindow();

        //Override
        LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);
    protected:

    private:
};

#endif // DRAWINGBOARDWINDOW_H
