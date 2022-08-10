#include <windows.h>
#include <gl/gl.h>
#include <math.h>

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

void DrawDots()
{
    // Dots at specific points in the coordinates.
    glPointSize(10);
    glBegin(GL_POINTS);
    glColor3f(0,0,0);
    glVertex2f(0.5, 0.5);
    glVertex2f(0.5, -0.5);
    glVertex2f(-0.5, 0.5);
    glVertex2f(-0.5, -0.5);
    glEnd();
}

void DrawLinesLoop()
{
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x00ff);
    glLineWidth(5);
    // lines with dots connected in a loop.
    glBegin(GL_LINE_LOOP);
    glColor3f(0,0,0);
    // Point a -> b
    glVertex2f(0.5, 0.5);
    glVertex2f(-0.5, 0.5);
    // Point c -> d
     glColor3f(1,0,0);
    glVertex2f(-0.5, -0.5);
    glVertex2f(0.5, -0.5);
    glEnd();
}

void DrawLinesStrip()
{
    // lines with dots connected consecutivelly.
    glBegin(GL_LINE_STRIP);
    glColor3f(0,0,0);
    // Point a -> b
    glVertex2f(0.5, 0.5);
    glVertex2f(-0.5, 0.5);
    // Point c -> d
    glVertex2f(-0.5, -0.5);
    glVertex2f(0.5, -0.5);
    glEnd();
}
void DrawLines()
{
    glLineWidth(10);
    glBegin(GL_LINES);
    glColor3f(0,0,0);
    // Point a -> b
    glVertex2f(0.5, 0.5);
    glVertex2f(-0.5, 0.5);
    // Point c -> d
    glVertex2f(-0.5, -0.5);
    glVertex2f(0.5, -0.5);
    glEnd();
}

void DrawTriangle()
{
    glBegin(GL_TRIANGLES);
    glColor3f(0,0,0);
    // Point a -> b
    glVertex2f(0.5, 0.5);
    glVertex2f(-0.5, 0.5);
    // Point c -> d
    glColor3f(0,0,1);
    glVertex2f(-0.5, -0.5);
    glEnd();
}
void DrawTriangleFan()
{
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0,0,0);
    // Point a -> b
    glVertex2f(0.5, 0.5);
    glVertex2f(-0.5, 0.5);
    // Point c -> d
    glColor3f(0,0,1);
    glVertex2f(-0.5, -0.5);
    glVertex2f(0.5, -0.5);
    glEnd();
}

void DrawShapesWithTriangles(int cnt)
{
    float x, y;
    //float cnt = 10;
    float l = 0.5;
    float a = M_PI * 2 / cnt;

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1,0,1);
    glVertex2f(0, 0);
    for(int i=-1; i<cnt; i++){
        x = sin(a * i) * l;
        y = cos(a * i) * l;
        glVertex2f(x, y);
    }
    glEnd();
}

void DrawName()
{
    glLineWidth(50);
    glBegin(GL_LINE_STRIP);
        glColor3f(1,0,0);
        //glVertex2f(0, 0.5);
        //glVertex2f(0, -0.5);

        //glVertex2f(sin(-1), cos(1));
        //glColor3f(0,0,1);
        glVertex2f(sin(1), cos(0));
        glVertex2f(sin(2), cos(3));
    glEnd();
}
/*
    The order of translation, rotation and scale. All matters
    in terms of the coordinate system.
    glScalef(0.3, 0.3, 0);
    glRotatef(45, 0, 0, 1);
    glTranslatef(0.5, 0, 0);
*/

void ShowObj()
{
     glBegin(GL_TRIANGLES);
        glColor3f(0, 0, 0);   glVertex2f(0, 0); /* Vertice*/
        glColor3f(1, 0, 0);   glVertex2f(0.5, 0);
        glColor3f(0, 1, 0);   glVertex2f(0, 0.5);
    glEnd();
}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;
    float theta = 0.0f;

    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "OpenGL Sample",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          500,
                          500,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);

    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            /* OpenGL animation code goes here */

            // light-green
            glClearColor(0.7f, 1.0f, 0.7f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // OPENGL Lesson 1:
            // point, line triangle and circle.

            // 1- Point
            //DrawDots();

            // 2- Lines
            //DrawLines();

            // 2.a Lines connected consecutivelly
            //DrawLinesStrip();

            // 2.b Lines connected in a loop
            //DrawLinesLoop();

            // 3- Triangle
            //DrawTriangle();

            // 3.a Triangles,
            // when the dots side each other
            // 4 dots will be enough to create a square like shape
            //
          //   DrawTriangleFan();

            // 3.b TriangleStrip // Use GL_TRIANGLE_STRIP
            // 3.c Hexagono
            // param = 10 = hex
            // param = 40 = circle
             DrawShapesWithTriangles(40);

            // Exercise 1: Draw your Name
            DrawLinesStrip();
            SwapBuffers(hDC);

            //theta += 1.0f;
            Sleep (1);
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                break;
            }
        }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}

