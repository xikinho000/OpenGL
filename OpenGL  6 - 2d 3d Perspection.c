#include <windows.h>
#include <gl/gl.h>

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

float vert [] = {1,1,0, -1,0,0, 1,-1,0};

float vert_ex [] = {1,1,0, -1,0,-2, 1,-1,0};
float vert_ex2 [] = {1,0,-1, -2,-1,0, -2,1,0};

void Orthogonal_Perspective_EX()
{
     // this will make it looks farther
    glTranslatef(0,0,-0.01);
    // go left in the x axis
    glVertexPointer(3, GL_FLOAT, 0, &vert);
    glEnableClientState(GL_VERTEX_ARRAY);
    glColor3f(0,1,0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
        glPushMatrix();
            glColor3f(0.9,0,1);
            glTranslatef(1,0,-1);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        glPopMatrix();
    glDisableClientState(GL_VERTEX_ARRAY);
}

void Ex_1()
{
  // this will make it looks farther
    glTranslatef(0,0,-0.01);
    // go left in the x axis
    glVertexPointer(3, GL_FLOAT, 0, &vert_ex);
    glEnableClientState(GL_VERTEX_ARRAY);
    glColor3f(0,1,0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
        glPushMatrix();
            glColor3f(0.9,0,1);
            glTranslatef(1,0,-1);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        glPopMatrix();
    glDisableClientState(GL_VERTEX_ARRAY);
}

void Ex_2()
{
    glTranslatef(0,0,-0.01);
    // go left in the x axis
    glVertexPointer(3, GL_FLOAT, 0, &vert_ex);
    glEnableClientState(GL_VERTEX_ARRAY);
    glColor3f(0,1,0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
        glPushMatrix();
            glColor3f(0.9,0,1);
            glTranslatef(1,0,-1);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        glPopMatrix();

        glVertexPointer(3, GL_FLOAT, 0, &vert_ex2);
        glColor3f(0.4, 0.5, 1);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableClientState(GL_VERTEX_ARRAY);

}


void Ex_3() // rotate
{
    glRotatef(2, 1, 0, 0);
    // go left in the x axis
    glVertexPointer(3, GL_FLOAT, 0, &vert_ex);
    glEnableClientState(GL_VERTEX_ARRAY);
    glColor3f(0,1,0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
        glPushMatrix();
            glColor3f(0.9,0,1);
            glTranslatef(1,0,-1);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        glPopMatrix();

        glVertexPointer(3, GL_FLOAT, 0, &vert_ex2);
        glColor3f(0.4, 0.5, 1);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableClientState(GL_VERTEX_ARRAY);

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

    glEnable(GL_DEPTH_TEST);
    // gl ortho 2d games, objects close and far apear same size.
    // gl frustum far objects looks smaller.
    glLoadIdentity();
    // frustum: orthogonal to perspective.
    // view elements with different sizes at different distances
    // the 2 and 6 and the camera view distance to plane

    // for all examples.
    //glFrustum(-1, 1, -1, 1, 2, 6);

    // for ex 3
    glFrustum(-1,1, -1,1, 2,100);
    glTranslatef(0,0,-6);
    // glOrtho: is only the view direction, no perspective applied
    //glOrtho(-2,2, -2,2, -1,1);
    //glOrtho(2,-2, -2,2, -1,1);
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
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //Ex_1();
            //Ex_2();
            //Ex_3();
            Orthogonal_Perspective_EX();
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

