#ifndef WINDOWSMANAGER_H
#define WINDOWSMANAGER_H

#include "header.h"

class WindowsManager
{
    public:
        WindowsManager(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow);
        virtual ~WindowsManager();

        HINSTANCE GethInstance() { return m_hInstance; }
        void SethInstance(HINSTANCE val) { m_hInstance = val; }
        HINSTANCE GethPrevInstance() { return m_hPrevInstance; }
        void SethPrevInstance(HINSTANCE val) { m_hPrevInstance = val; }
        LPSTR GetlpszCmdParam() { return m_lpszCmdParam; }
        void SetlpszCmdParam(LPSTR val) { m_lpszCmdParam = val; }
        int GetnCmdShow() { return m_nCmdShow; }
        void SetnCmdShow(int val) { m_nCmdShow = val; }

        MainWindow* GetpMainWindow() { return m_MainWindow; }
        ToolBarWindow* GetpToolBarWindow() { return m_ToolBarWindow; }
        DrawingBoardWindow* GetpDrawingBoardWindow() { return m_DrawingBoardWindow; }

        void createWindows();
        void runMessageLoops();
    protected:

    private:
        HINSTANCE m_hInstance;
        HINSTANCE m_hPrevInstance;
        LPSTR m_lpszCmdParam;
        int m_nCmdShow;

        MainWindow *m_pMainWindow;
        ToolBarWindow *m_pToolBarWindow;
        DrawingBoardWindow *m_pDrawingBoardWindow;
};

#endif // WINDOWSMANAGER_H
