#include <windows.h>
#include <gl/gl.h>

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);
// Ex. letter z
float masZ[] = {0.25,1, 0.75,1, 0.25,0, 0.75,0}; // for draw arrays
GLuint masZi[] = {0,1, 1,2, 2,3}; // for elements

//Christhmas tree
float elka[] = {
-0.25, 0.5, -0.25, 0, 0.25, 0,
0.25, 0, 0.25, 0.5, -0.25, 0.5,
-1,0.5, 1,0.5, 0,1.2,
-0.75, 1, 0.75,1, 0, 1.7,
-0.5, 1.5, 0.5,1.5, 0,2
};
//colors
float elkaCol[] = {
    0.58,0.26,0.05, 0.58,0.26,0.05, 0.58,0.26,0.05,
    0.58,0.26,0.05, 0.58,0.26,0.05, 0.58,0.26,0.05,
    0.05,0.53,0.05, 0.05,0.53,0.05, 0.05,0.53,0.05,
    0.05,0.53,0.05, 0.05,0.53,0.05, 0.05,0.53,0.05,
    0.05,0.53,0.05, 0.05,0.53,0.05, 0.05,0.53,0.05
};

float elka2[] = {
-0.25, 0.5, -0.25,0,
0.25, 0, 0.25, 0.5,
-1,0.5, 1,0.5, 0,1.2,
-0.75, 1, 0.75,1, 0, 1.7,
-0.5, 1.5, 0.5,1.5, 0,2
};
//colors
float elkaCol2[] = {
    0.58,0.26,0.05, 0.58,0.26,0.05,
    0.58,0.26,0.05, 0.58,0.26,0.05,
    0.05,0.53,0.05, 0.05,0.53,0.05, 0.05,0.53,0.05,
    0.05,0.53,0.05, 0.05,0.53,0.05, 0.05,0.53,0.05,
    0.05,0.53,0.05, 0.05,0.53,0.05, 0.05,0.53,0.05
};
GLuint elkaInd[] = {0,1,2, 2,3,0, 4,5,6, 7,8,9, 10,11,12};

void DrawZ_Elements()
{
    glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, &masZ);
        glColor3f(0,1,0);
        glLineWidth(3);
        glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, &masZi);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void DrawZ_Vertex()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, &masZ);
    glColor3f(0,1,0);
    glLineWidth(3);
    glDrawArrays(GL_LINE_STRIP, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void DrawTree() // using vertex array
{
    glPushMatrix();
    glTranslatef(0, -1, 0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, &elka);
        glColorPointer(3, GL_FLOAT, 0, &elkaCol);
        glColor3f(0,1,0);
        glDrawArrays(GL_TRIANGLES, 0, 15);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    glPopMatrix();
}

void DrawTree2() // using elements
{
    glPushMatrix();
    glTranslatef(0, -1, 0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, &elka2);
        glColorPointer(3, GL_FLOAT, 0, &elkaCol2);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, &elkaInd);
        //glDrawElements(GL_TRIANGLES, 15, GL_UNSIGNED_INT, &elkaInd);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glPopMatrix();
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
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            //Ex. 1 : Draw Letter Z Using Vertex Arrays
            //DrawZ_Vertex();
            //Ex. 2 : Draw Letter Z Using Element Array
            //DrawZ_Elements();
            //Ex. 3 : Draw Chrithmas Tree using vertex array
            //DrawTree();
            //Ex. 4 : Draw Chrithmas Tree using elements array
            DrawTree2();
            SwapBuffers(hDC);
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

