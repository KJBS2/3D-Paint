#include "WindowsManager.h"

WindowsManager::WindowsManager(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
    // get WinMain() function's parameters
    m_hInstance = hInstance;
    m_hPrevInstance = hPrevInstance;
    m_lpszCmdParam = lpszCmdParam;
    m_nCmdShow = nCmdShow;

    // initiate Window Objects
    m_pMainWindow = new MainWindow(this);
    m_pToolBarWindow = new ToolBarWindow();
    m_pDrawingBoardWindow = new DrawingBoardWindow();
}

WindowsManager::~WindowsManager()
{
    //dtor
}

void
WindowsManager::createWindows()
{
    WNDCLASS *WndClass = nullptr;

    // Register MainWindow
    WndClass = &(m_MainWindow->GetWndClass());

    WndClass->cbClsExtra=0;
    WndClass->cbWndExtra=0;
    WndClass->hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
    WndClass->hCursor=LoadCursor(NULL, IDC_ARROW);
    WndClass->hInstance=m_hInstance;
    WndClass->lpfnWndProc=(WNDPROC)(m_MainWindow->WndProc);
    WndClass->lpszClassName=(m_MainWindow->GetlpszClass());
    WndClass->lpszMenuName=NULL;
    WndClass->style=CS_HREDRAW | CS_VREDRAW;
    RegisterClass(WndClass);

    // Register ToolBarWindow
    WndClass->hbrBackground=(HBRUSH)CreateSolidBrush(RGB(255, 0, 0));
    WndClass->lpfnWndProc=(WNDPROC)(m_ToolBarWindow->WndProc);
    WndClass->lpszClassName=(m_ToolBarWindow->GetlpszClass());

    // Register DrawingBoardWindow
    WndClass->hbrBackground=(HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
    WndClass->lpfnWndProc=(WNDPROC)(m_DrawingBoardWindow->WndProc);
    WndClass->lpszClassName=(m_DrawingBoardWindow->GetlpszClass());

    // Create All Windows
    m_MainWindow->SethWnd(CreateWindow(m_MainWindow->GetlpszClass(),
                                       m_MainWindow->GetlpszClass(),
                                       WS_OVERLAPPEDWINDOW,
                                       CW_USEDEFAULT,
                                       CW_USEDEFAULT,
                                       CW_USEDEFAULT,
                                       CW_USEDEFAULT,
                                       NULL,
                                       (HMENU)NULL,
                                       m_hInstance,
                                       NULL));
}

void WindowsManager::runMessageLoops()
{
    MSG Message;

    while(GetMessage(&Message, 0, 0, 0)){
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }
}
