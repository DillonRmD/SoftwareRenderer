#include "render.h"

#include <windows.h>

struct win32_window_size{
    s32 Width, Height;
};

global bool GlobalRunning;
global BITMAPINFO Win32GlobalBitmapInfo;
global frame_buffer Win32GlobalFrameBuffer;

internal win32_window_size Win32GetWindowSize(HWND Window)
{
    win32_window_size Result = {};

    RECT Win32Rect;
    GetClientRect(Window, &Win32Rect);
    Result.Width = Win32Rect.right - Win32Rect.left;
    Result.Height = Win32Rect.bottom - Win32Rect.top;

    return Result;
}

internal void Win32AllocateFrameBuffer(frame_buffer* FrameBuffer, s32 Width, s32 Height)
{
     // NOTE(Dillon): Check to see if there is anything at that memory location,
     // if so free it, else continue
    if(FrameBuffer->Memory)
    {
        free(FrameBuffer->Memory);
    }

    FrameBuffer->Width = Width;
    FrameBuffer->Height = Height;

    Win32GlobalBitmapInfo.bmiHeader.biSize = sizeof(Win32GlobalBitmapInfo);
    Win32GlobalBitmapInfo.bmiHeader.biWidth = FrameBuffer->Width;
    Win32GlobalBitmapInfo.bmiHeader.biHeight = -FrameBuffer->Height; //IMPORTANT(Dillon): We flip the height so that (0, 0) is top left
    Win32GlobalBitmapInfo.bmiHeader.biPlanes = 1;
    Win32GlobalBitmapInfo.bmiHeader.biBitCount = 32;
    Win32GlobalBitmapInfo.bmiHeader.biCompression = BI_RGB;

    FrameBuffer->BytesPerPixel = 4;
    FrameBuffer->Pitch = FrameBuffer->BytesPerPixel * FrameBuffer->Width;
    u32 MemorySize = (FrameBuffer->Width * FrameBuffer->Height) * FrameBuffer->BytesPerPixel;
    FrameBuffer->Memory = (void*)malloc(MemorySize);
}

internal void Win32DisplayFrameBuffer(HDC DeviceContext, frame_buffer FrameBuffer, u32 Width, u32 Height)
{

    StretchDIBits(DeviceContext, 
    0, 0, 
    Width, Height, 
    0, 0,
    FrameBuffer.Width, FrameBuffer.Height, 
    FrameBuffer.Memory,
    &Win32GlobalBitmapInfo,
    DIB_RGB_COLORS,
    SRCCOPY);
}

internal LRESULT CALLBACK Win32Procedure(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam){

    switch(Message){
        case WM_CLOSE:
        {
            GlobalRunning = false;
        }break;
        case WM_SIZE:
        {
            win32_window_size Size = Win32GetWindowSize(Window);
            Win32AllocateFrameBuffer(&Win32GlobalFrameBuffer, Size.Width, Size.Height);
        }break;
        case WM_PAINT:
        {
            PAINTSTRUCT PaintStruct;
            win32_window_size Size = Win32GetWindowSize(Window);
            HDC DeviceContext = BeginPaint(Window, &PaintStruct);
            Win32DisplayFrameBuffer(DeviceContext, Win32GlobalFrameBuffer, Size.Width, Size.Height);
            EndPaint(Window, &PaintStruct);
        }break;
        default:{
            return DefWindowProc(Window, Message, WParam, LParam);
        }break;
    }

    return DefWindowProc(Window, Message, WParam, LParam);
}

int WINAPI WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR CommandLine, int CommandShow)
{
    WNDCLASS WindowClass = {0};
    WindowClass.hInstance = Instance;
    WindowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    WindowClass.lpfnWndProc = Win32Procedure;
    WindowClass.lpszClassName = TEXT("Fuck dude");
    WindowClass.hCursor = LoadCursor(0, IDC_ARROW);
    WindowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    
    if(!RegisterClass(&WindowClass))
    {
        // TODO(Dillon): Logging
    }
    
    HWND Window = CreateWindow(WindowClass.lpszClassName, WindowClass.lpszClassName,
                               WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                               CW_USEDEFAULT, CW_USEDEFAULT,
                               1280, 720,
                               0, 0, WindowClass.hInstance, 0);
    
    if(!Window)
    {
        // TODO(Dillon): Logging
    }
    
    HDC DeviceContext = GetDC(Window);
    
    win32_window_size WindowSize = Win32GetWindowSize(Window);
    Win32AllocateFrameBuffer(&Win32GlobalFrameBuffer, WindowSize.Width, WindowSize.Height);

    GlobalRunning = true;
    UpdateWindow(Window);
    ShowWindow(Window, CommandShow);

    MSG Message;
    while(GlobalRunning)
    {
        while(PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&Message);
            DispatchMessage(&Message);
        }

        WindowSize = Win32GetWindowSize(Window);
        HDC DeviceContext = GetDC(Window);

        Render(&Win32GlobalFrameBuffer);

        Win32DisplayFrameBuffer(DeviceContext, Win32GlobalFrameBuffer, WindowSize.Width, WindowSize.Height);
        ReleaseDC(Window, DeviceContext);
    }

    return 0;
}