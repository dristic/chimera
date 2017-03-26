// Copyright 2016 Dan Ristic

#include "Cosmonaut/Cosmonaut.h"

#include "app/login.h"
#include "app/loading.h"

#include <Windows.h>
#include <windowsx.h>

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const int WIDTH = 1280;
    const int HEIGHT = 720;

    Nova::Context context;
    Nova::Document& document = context.document;

    OutputDebugString(TEXT("Starting..."));

    // the handle for the window, filled by a function
    HWND hWnd;
    // this struct holds information for the window class
    WNDCLASSEX wc;

    // clear out the window class for use
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    // fill in the struct with the needed information
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = TEXT("WindowClass1");

    // register the window class
    RegisterClassEx(&wc);

    // create the window and use the result as the handle
    hWnd = CreateWindowEx(NULL,
        TEXT("WindowClass1"),    // name of the window class
        TEXT("Our First Windowed Program"),   // title of the window
        WS_OVERLAPPEDWINDOW,    // window style
        300,    // x-position of the window
        300,    // y-position of the window
        500,    // width of the window
        400,    // height of the window
        NULL,    // we have no parent window, NULL
        NULL,    // we aren't using menus, NULL
        hInstance,    // application handle
        NULL);    // used with multiple windows, NULL

                  // display the window on the screen
    ShowWindow(hWnd, nCmdShow);

    // enter the main loop:

    // this struct holds Windows event messages
    MSG msg;

    // Enter the infinite message loop
    while(true)
    {
        // Check to see if any messages are waiting in the queue
        while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            // translate keystroke messages into the right format
            TranslateMessage(&msg);

            // send the message to the WindowProc function
            DispatchMessage(&msg);
        }

        // If the message is WM_QUIT, exit the while loop
        if (msg.message == WM_QUIT)
            break;

        // Run game code here
        // ...
        // ...
    }

    // return this part of the WM_QUIT message to Windows
    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // sort through and find what code to run for the message given
    switch (message)
    {
        // this message is read when the window is closed
    case WM_DESTROY:
    {
        // close the application entirely
        PostQuitMessage(0);
        return 0;
    } break;
    }

    // Handle any messages the switch statement didn't
    return DefWindowProc(hWnd, message, wParam, lParam);
}
