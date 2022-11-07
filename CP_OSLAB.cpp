/*
 * Dependencies:
 *  gdi32
 *  (kernel32)
 *  user32
 *  (comctl32)
 *
 */

 //LAB1

#include <fstream>
#include <string>
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
int n, width, height, R, G, B, r, g, b, w, h;
int** m;
/* Runs Notepad */

void RunNotepad(void)
{
    STARTUPINFO sInfo;
    PROCESS_INFORMATION pInfo;

    ZeroMemory(&sInfo, sizeof(STARTUPINFO));

    puts("Запуск блокнота...");
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
            DeleteObject(hPen);

        }
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
            {
                if (m[i][j] == 1)
                {
                    SelectObject(hdc, GetStockObject(NULL_BRUSH));
                    hPen = CreatePen(0, 6, RGB(0, 0, 128));
                    SelectObject(hdc, hPen);
                    if (w <= h)
                    {
                        Ellipse(hdc, (w * i) + 0.1 * w, (h * j + (h - w) / 2) + 0.1 * w, (w * (i + 1)) - 0.1 * w, (h * (j + 1) - (h - w) / 2) - 0.1 * w);
                    }
                    else
                    {
                        Ellipse(hdc, (w * i + (w - h) / 2) + 0.1 * h, (h * j) + 0.1 * h, (w * (i + 1) - (w - h) / 2) - 0.1 * h, (h * (j + 1)) - 0.1 * h);
                    }
                    DeleteObject(hPen);
                }
                else
                {
                    if (m[i][j] == 2)
                    {
                        SelectObject(hdc, GetStockObject(NULL_BRUSH));
                        hPen = CreatePen(0, 6, RGB(0, 128, 0));
                        SelectObject(hdc, hPen);
                        if (w <= h)
                        {
                            MoveToEx(hdc, (w * i) + 0.1 * w, (h * j + (h - w) / 2) + 0.1 * w, NULL);
                            LineTo(hdc, (w * (i + 1)) - 0.1 * w, (h * (j + 1) - (h - w) / 2) - 0.1 * w);
                            MoveToEx(hdc, (w * (i + 1)) - 0.1 * w, (h * j + (h - w) / 2) + 0.1 * w, NULL);
                            LineTo(hdc, (w * i) + 0.1 * w, (h * (j + 1) - (h - w) / 2) - 0.1 * w);
                        }
                        else
                        {
                            MoveToEx(hdc, (w * i + (w - h) / 2) + 0.1 * h, (h * j) + 0.1 * h, NULL);
                            LineTo(hdc, (w * (i + 1) - (w - h) / 2) - 0.1 * h, (h * (j + 1)) - 0.1 * h);
                            MoveToEx(hdc, (w * i + (w - h) / 2) + 0.1 * h, h * (j + 1) - 0.1 * h, NULL);
                            LineTo(hdc, (w * (i + 1) - (w - h) / 2) - 0.1 * h, (h * j) + 0.1 * h);
                        }
                        DeleteObject(hPen);
                    }
                }
            }






        GetWindowRect(hwnd, &reClient);
        width = reClient.right - reClient.left;
        height = reClient.bottom - reClient.top;
        EndPaint(hwnd, &ps);
        DeleteObject(hPen);
    }
    return 0;
    case WM_LBUTTONUP:
    {

        if (m[GET_X_LPARAM(lParam) / w][GET_Y_LPARAM(lParam) / h] == 0)
            m[GET_X_LPARAM(lParam) / w][GET_Y_LPARAM(lParam) / h] = 1;
        InvalidateRect(hwnd, NULL, TRUE);
        return 0;
    }
    case WM_RBUTTONUP:
    {
        if (m[GET_X_LPARAM(lParam) / w][GET_Y_LPARAM(lParam) / h] == 0)
            m[GET_X_LPARAM(lParam) / w][GET_Y_LPARAM(lParam) / h] = 2;
        InvalidateRect(hwnd, NULL, TRUE);
        return 0;
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
            R = rand() % 255;
            G = rand() % 255;
            B = rand() % 255;
            hBrush = CreateSolidBrush(RGB(R, G, B));
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
    ifstream values;
    values.open("values.txt");
    if (!values.is_open())
    {
        if (argc < 2)
            n = 3;
        else
            n = atoi(argv[1]);
        width = 320;
        height = 240;
        R = 0;
        G = 0;
        B = 255;
        r = 255;
        g = 0;
        b = 0;
    }
    else
    {
        string s;
        getline(values, s);
        if (argc < 2)
            n = stoi(s);
        else
            n = atoi(argv[1]);
        getline(values, s);
        width = stoi(s);
        getline(values, s);
        height = stoi(s);
        getline(values, s);
        R = stoi(s);
        getline(values, s);
        G = stoi(s);
        getline(values, s);
        B = stoi(s);
        getline(values, s);
        r = stoi(s);
        getline(values, s);
        g = stoi(s);
        getline(values, s);
        b = stoi(s);
        values.close();
    }


    m = new int* [n];
    for (int i = 0; i < n; i++)
        m[i] = new int[n];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            m[i][j] = 0;
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
    wincl.hCursor = LoadCursor(NULL, IDC_ARROW); // class cursor 

    /* Use custom brush to paint the background of the window */
    hBrush = CreateSolidBrush(RGB(R, G, B));
    wincl.hbrBackground = hBrush;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClass(&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindow(
        szWinClass,          /* Classname */
        szWinName,       /* Title Text */
        WS_OVERLAPPEDWINDOW, /* default window */
        300,       /* Windows decides the position */
        300,       /* where the window ends up on the screen */
        width,                 /* The programs width */
        height,                 /* and height in pixels */
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
    ofstream valuesout;
    valuesout.open("values.txt");
    valuesout << n;
    valuesout << "\n";
    valuesout << width;
    valuesout << "\n";
    valuesout << height;
    valuesout << "\n";
    valuesout << R;
    valuesout << "\n";
    valuesout << G;
    valuesout << "\n";
    valuesout << B;
    valuesout << "\n";
    valuesout << r;
    valuesout << "\n";
    valuesout << g;
    valuesout << "\n";
    valuesout << b;
    valuesout << "\n";
    /* Cleanup stuff */
    valuesout.close();
    for (int i = 0; i < n; i++)
        delete[] m[i];
    delete[] m;
    DestroyWindow(hwnd);
    UnregisterClass(szWinClass, hThisInstance);
    DeleteObject(hBrush);
    //RunNotepad();
    //cout << argv[0];
    return 0;
}
