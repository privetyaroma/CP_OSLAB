/*
 * Dependencies:
 *  gdi32
 *  (kernel32)
 *  user32
 *  (comctl32)
 *
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <windows.h>
#include <locale.h>
#include <time.h>
#include <windowsx.h>
using namespace std;

#define KEY_SHIFTED     0x8000
#define KEY_TOGGLED     0x0001
#define KEY_CTRL        0x11
#define KEY_Q           0x51
#define KEY_ESC         0x1B
#define KEY_SHIFT       0x10
#define KEY_C           0x43
#define KEY_ENTER       0x0D

const TCHAR szWinClass[] = _T("Win32SampleApp");
const TCHAR szWinName[] = _T("Крестики-нолики");
HWND hwnd;               /* This is the handle for our window */
HBRUSH hBrush;           /* Current brush */
int n = 3, r = 255, g = 0, b = 0, w, h;

/* Runs Notepad */

void RunNotepad(void)
{
    STARTUPINFO sInfo;
    PROCESS_INFORMATION pInfo;

    ZeroMemory(&sInfo, sizeof(STARTUPINFO));

    puts("Starting Notepad...");
    CreateProcess(_T("C:\\Windows\\Notepad.exe"),
        NULL, NULL, NULL, FALSE, 0, NULL, NULL, &sInfo, &pInfo);
}

/*  This function is called by the Windows function DispatchMessage()  */
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HPEN hPen;
        RECT reClient;
        GetClientRect(hwnd, &reClient);
        HDC hdc = BeginPaint(hwnd, &ps);
        hPen = CreatePen(PS_SOLID, 3, RGB(r, g, b));
        SelectObject(hdc, hPen);
        w = (reClient.right - reClient.left) / n;
        h = (reClient.bottom - reClient.top) / n;
        for (int i = 1; i < n; i++)
        {
            MoveToEx(hdc, w * i, reClient.top, (LPPOINT)NULL);
            LineTo(hdc, w * i, reClient.bottom);
            MoveToEx(hdc, reClient.left, h * i, NULL);
            LineTo(hdc, reClient.right, h * i);

        }
        EndPaint(hwnd, &ps);
        DeleteObject(hPen);
    }
    return 0;
    case WM_LBUTTONUP:
    {

        int j = GET_Y_LPARAM(lParam) / h + 1;
        int i = GET_X_LPARAM(lParam) / w + 1;
        cout << j << " " << i << endl;
        break;
    }
    case WM_MOUSEWHEEL:
    {    if ((short)HIWORD(wParam) < 0)

    {

        r -= 10;
        g -= 20;
        b -= 30;
        InvalidateRect(hwnd, NULL, TRUE);
    }
    else
    {

        r += 10;
        g += 20;
        b += 30;
        InvalidateRect(hwnd, NULL, TRUE);
    }
    break;
    }
    case WM_KEYUP:
        switch (wParam)
        {


        case KEY_Q:
            if (GetKeyState(KEY_CTRL) & 0x8000)
                PostQuitMessage(0);
            break;

        case KEY_ESC:
            PostQuitMessage(0);
            break;

        case KEY_C:
            if (GetKeyState(KEY_SHIFT) & 0x8000)
                RunNotepad();
            break;

        case KEY_ENTER:
        {
            DeleteObject(hBrush);
            hBrush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
            SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG)hBrush);
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        }

        }

        return 0;
    case WM_SIZE:
    {
        InvalidateRect(hwnd, NULL, TRUE);
        return 0;
    }
    case WM_DESTROY:
        PostQuitMessage(0);       /* send a WM_QUIT to the message queue */
        return 0;


    }
    /* for messages that we don't deal with */
    return DefWindowProc(hwnd, message, wParam, lParam);
}

int main(int argc, char** argv)
{
    srand(time(NULL));
    setlocale(LC_ALL, "Rus");
    BOOL bMessageOk;
    MSG message;            /* Here message to the application are saved */
    WNDCLASS wincl = { 0 };         /* Data structure for the windowclass */

    /* Harcode show command num when use non-winapi entrypoint */
    int nCmdShow = SW_SHOW;
    /* Get handle */
    HINSTANCE hThisInstance = GetModuleHandle(NULL);

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szWinClass;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by Windows */

    /* Use custom brush to paint the background of the window */
    hBrush = CreateSolidBrush(RGB(0, 0, 255));
    wincl.hbrBackground = hBrush;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClass(&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindow(
        szWinClass,          /* Classname */
        szWinName,       /* Title Text */
        WS_OVERLAPPEDWINDOW, /* default window */
        CW_USEDEFAULT,       /* Windows decides the position */
        CW_USEDEFAULT,       /* where the window ends up on the screen */
        320,                 /* The programs width */
        240,                 /* and height in pixels */
        HWND_DESKTOP,        /* The window is a child-window to desktop */
        NULL,                /* No menu */
        hThisInstance,       /* Program Instance handler */
        NULL                 /* No Window Creation data */
    );

    /* Make the window visible on the screen */
    ShowWindow(hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while ((bMessageOk = GetMessage(&message, NULL, 0, 0)) != 0)
    {
        /* Yep, fuck logic: BOOL mb not only 1 or 0.
         * See msdn at https://msdn.microsoft.com/en-us/library/windows/desktop/ms644936(v=vs.85).aspx
         */
        if (bMessageOk == -1)
        {
            puts("Suddenly, GetMessage failed! You can call GetLastError() to see what happend");
            break;
        }
        /* Translate virtual-key message into character message */
        TranslateMessage(&message);
        /* Send message to WindowProcedure */
        DispatchMessage(&message);
    }

    /* Cleanup stuff */
    DestroyWindow(hwnd);
    UnregisterClass(szWinClass, hThisInstance);
    DeleteObject(hBrush);

    //RunNotepad();

    return 0;
}
