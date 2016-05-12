#include <windows.h>
#include <windowsx.h>
#include "WindowsManager.h"
#include <new>

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                     LPSTR lpszCmdParam, int nCmdShow)
{
    // Initiate WindowsManager instance
    WindowsManager *pWindowsManager = new WindowsManager(hInstance, hPrevInstance, lpszCmdParam, nCmdShow);

    // Create Windows
    pWindowsManager->createWindows();

    // Run Message Loops of Windows
    pWindowsManager->runMessageLoops();

    return 0;
}
