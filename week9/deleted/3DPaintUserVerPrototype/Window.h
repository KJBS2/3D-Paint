#ifndef WINDOW_H
#define WINDOW_H

#include "header.h"

class Window
{
    public:
        Window();
        virtual ~Window();

        LPSTR GetlpszClass() { return m_lpszClass; }
        void SetlpszClass(LPSTR val) { m_lpszClass = val; }
        HWND GethWnd() { return m_hWnd; }
        void SethWnd(HWND val) { m_hWnd = val; }
        WNDCLASS GetWndClass() { return m_WndClass; }
        void SetWndClass(WNDCLASS val) { m_WndClass = val; }

        virtual LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM) = 0;
    protected:

    private:
        LPSTR m_lpszClass;
        HWND m_hWnd;
        WNDCLASS m_WndClass;
};

#endif // WINDOW_H
