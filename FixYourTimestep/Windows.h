// Simple Windows OpenGL framework

#ifdef _WIN32

#define VC_EXTRALEAN 
#define WIN32_LEAN_AND_MEAN 

#include <windows.h>
#include <windowsx.h>

#include <gl/gl.h>
#include <gl/glu.h>

HWND window = 0;
HDC device = 0;
HGLRC context = 0;
bool active = true;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_PAINT:
            ValidateRect(hWnd, NULL);
            break;

        case WM_KEYDOWN:
            if ((wParam&0xFF)==VK_ESCAPE)
                onQuit();
            break;

        case WM_CLOSE:
            onQuit();
            break;

        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}

bool openDisplay(const char title[], int width, int height)
{
    HINSTANCE instance = GetModuleHandle(0);

    WNDCLASSEX windowClass;
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_OWNDC;
    windowClass.lpfnWndProc = &WndProc;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = instance;
    windowClass.hIcon = 0;
    windowClass.hCursor = LoadCursor(NULL,IDC_ARROW);
    windowClass.hbrBackground = NULL;
    windowClass.lpszMenuName = NULL;
    windowClass.lpszClassName = title;
    windowClass.hIconSm = NULL;

    UnregisterClass(title, instance);

    if (!RegisterClassEx(&windowClass)) 
        return false;

    // create window

    RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = width;
    rect.bottom = height;
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, 0);
    rect.right -= rect.left;
    rect.bottom -= rect.top;

    int x = (GetSystemMetrics(SM_CXSCREEN) - rect.right) >> 1;
    int y = (GetSystemMetrics(SM_CYSCREEN) - rect.bottom) >> 1;

    window = CreateWindow( title, title, WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME, 
                           x, y, rect.right, rect.bottom,
                           NULL, NULL, instance, NULL );

    if (!window)
        return false;

    // initialize wgl

    PIXELFORMATDESCRIPTOR descriptor;
    memset(&descriptor, 0, sizeof(descriptor));
    descriptor.nVersion = 1;
    descriptor.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    descriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    descriptor.iPixelType = PFD_TYPE_RGBA;
    descriptor.cColorBits = 32;
    descriptor.cDepthBits = 16;
    descriptor.iLayerType = PFD_MAIN_PLANE;

    device = GetDC(window);
    if (!device)
        return false;

    GLuint format = ChoosePixelFormat(device, &descriptor);
    if (!format)
        return false;

    if (!SetPixelFormat(device, format, &descriptor))
        return false;

    context = wglCreateContext(device);
    if (!context)
        return false;

    if (!wglMakeCurrent(device, context))
        return false;

    // show window

    ShowWindow(window, SW_NORMAL);

    return true;
}

void updateDisplay()
{
    // process window messages

    MSG message;

    while (true)
    {
        if (active)
        {
            if (!PeekMessage(&message, window, 0, 0, PM_REMOVE)) 
                break;

            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        else
        {
            if (!GetMessage(&message, window, 0, 0)) 
                break;

            TranslateMessage(&message);
            DispatchMessage(&message);
        }
    }

    // show rendering

    SwapBuffers(device);
}

void closeDisplay()
{	
    ReleaseDC(window, device);
    device = 0;

    DestroyWindow(window);
    window = 0;
}

float time()
{
    static __int64 start = 0;
    static __int64 frequency = 0;

    if (start==0)
    {
        QueryPerformanceCounter((LARGE_INTEGER*)&start);
        QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
        return 0.0f;
    }

    __int64 counter = 0;
    QueryPerformanceCounter((LARGE_INTEGER*)&counter);
    return (float) ((counter - start) / double(frequency));
}

// naughty! :)

int main();

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
   main();
}

#endif
