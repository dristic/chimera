// Copyright 2016 Dan Ristic

#include "Chimera/Chimera.h"

#include "src/adaptor/dx9.h"
#include "app/root.h"

#include <Windows.h>
#include <windowsx.h>
#include <d3d9.h>

// include the Direct3D Library file
#pragma comment (lib, "d3d9.lib")

// define the screen resolution
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

// global declarations
LPDIRECT3D9 d3d;    // the pointer to our Direct3D interface
LPDIRECT3DDEVICE9 d3ddev;    // the pointer to the device class

Chimera::Context context;

void initD3D(HWND hWnd);    // sets up and initializes Direct3D
void cleanD3D(void);    // closes Direct3D and releases memory

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    CHIMERA_UNUSED(hPrevInstance);
    CHIMERA_UNUSED(lpCmdLine);

    const int WIDTH = 1280;
    const int HEIGHT = 720;

    Chimera::Document& document = context.document;

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
    wc.lpszClassName = TEXT("Cosmonaut");

    // register the window class
    RegisterClassEx(&wc);

    RECT windowRect{ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);

    // create the window and use the result as the handle
    hWnd = CreateWindowEx(NULL,
        TEXT("Cosmonaut"),    // name of the window class
        TEXT("CosmonautDX9 Example"),   // title of the window
        WS_OVERLAPPEDWINDOW,    // window style
        300,    // x-position of the window
        300,    // y-position of the window
        windowRect.right - windowRect.left,    // width of the window
        windowRect.bottom - windowRect.top,    // height of the window
        NULL,    // we have no parent window, NULL
        NULL,    // we aren't using menus, NULL
        hInstance,    // application handle
        NULL);    // used with multiple windows, NULL

                  // display the window on the screen
    ShowWindow(hWnd, nCmdShow);

    initD3D(hWnd);

    document.setDimensions(WIDTH, HEIGHT);
    document.setScreenDimensions(WIDTH, HEIGHT);

    auto adaptor = std::make_shared<Chimera::DX9Adaptor>(d3ddev);
    context.useAdaptor(adaptor);

    context.loadFont("Roboto", "assets/Roboto-Regular.ttf");
    context.loadFont("Roboto Thin", "assets/Roboto-Thin.ttf");

    document.body->append(document.createElement<AppElement>());

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

        // clear the window to a black
        d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

        d3ddev->BeginScene();    // begins the 3D scene

        context.update(16);
        context.render(16);

        d3ddev->EndScene();    // ends the 3D scene

        d3ddev->Present(NULL, NULL, NULL, NULL);    // displays the created frame
    }

    cleanD3D();

    // return this part of the WM_QUIT message to Windows
    return msg.wParam;
}

// this function initializes and prepares Direct3D for use
void initD3D(HWND hWnd)
{
    d3d = Direct3DCreate9(D3D_SDK_VERSION);    // create the Direct3D interface

    D3DPRESENT_PARAMETERS d3dpp;    // create a struct to hold various device information

    ZeroMemory(&d3dpp, sizeof(d3dpp));    // clear out the struct for use
    d3dpp.Windowed = TRUE;    // program windowed, not fullscreen
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;    // discard old frames
    d3dpp.hDeviceWindow = hWnd;    // set the window to be used by Direct3D
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;    // set the back buffer format to 32-bit
    d3dpp.BackBufferWidth = SCREEN_WIDTH;    // set the width of the buffer
    d3dpp.BackBufferHeight = SCREEN_HEIGHT;    // set the height of the buffer

    // create a device class using this information and information from the d3dpp stuct
    d3d->CreateDevice(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hWnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &d3dpp,
        &d3ddev);
}

// this is the function that cleans up Direct3D and COM
void cleanD3D(void)
{
    d3ddev->Release();    // close and release the 3D device
    d3d->Release();    // close and release Direct3D
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

    case WM_MOUSEMOVE:
    {
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);

        context.setMousePosition(xPos, yPos);
    } break;

    case WM_LBUTTONDOWN:
    {
        context.setMouseDown();
    } break;

    case WM_CHAR:
    {
        char value = static_cast<char>(wParam);
        std::string chars{ value };
        context.addInputCharacters(chars);
    } break;
    }

    // Handle any messages the switch statement didn't
    return DefWindowProc(hWnd, message, wParam, lParam);
}
