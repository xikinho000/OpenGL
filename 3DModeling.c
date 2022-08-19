// http://www.csc.villanova.edu/~mdamian/Past/graphicssp13/Assignments.html
#include <windows.h>
#include <gl/gl.h>
#include <math.h>
#include <stdio.h>

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

float vert [] = {1,1,0, 1,-1,0, -1,-1,0, -1,1,0};
float xAlfa = 20;
float zAlfa  = 0;
float yAlfa  = 0;
POINTFLOAT pos = {0,0};
float pyramid [] = {0,0,2, 1,1,0, 1,-1,0, -1,-1,0, -1,1,0, 1,1,0};

// void perspectiveGL( GLdouble fieldOfView, GLdouble aspect, GLdouble zNear, GLdouble zFar )
// {
//     int width = 1500;
//     int height = 800;
//     glLoadIdentity();
//     float aspect = float(width)/float(height);
//     GLfloat fH = tan( float(fieldOfView / 360.0f * M_PI) ) * zNear;
//     GLfloat fW = fH * aspect;
//     glFrustum( -fW, fW, -fH, fH, zNear, zFar );
// }


void DrawPointer()
{
    // for the excercise
    glVertexPointer(3, GL_FLOAT, 0, &pyramid);
    glColor3f(1,0,0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

    glPushMatrix();
        glTranslatef(-3,3,0);
        glRotatef(45, 0,0,1);
        glColor3f(0,0,1);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(3,3,0);
        glScalef(1, 1, 0.5);
        glColor3f(1,0.6,0);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
    glPopMatrix();
}

void DrawGrid()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &vert);
    for(int i = -5; i <= 5; i++){
        for(int j = -5; j <= 5; j++){
            glPushMatrix();
                glColor3f(1, 0, 0);
                glTranslatef(i*2, j*2, 0);
                glDrawArrays(GL_LINES, 0, 4);
            glPopMatrix();
        }
    }
    glRotatef(90, 0, 0, 1);
    for(int i = -5; i <= 5; i++){
        for(int j = -5; j <= 5; j++){
            glPushMatrix();
                glColor3f(1, 0, 0);
                glTranslatef(i*2, j*2, 0);
                glDrawArrays(GL_LINES, 0, 4);
            glPopMatrix();
        }
    }
    glDisableClientState(GL_VERTEX_ARRAY);
}

void MoveCamera()
{
    // For Angle move.
    if(GetKeyState(VK_UP) < 0){
        xAlfa = ++xAlfa > 180 ? 180 : xAlfa; // look up
    }
    if(GetKeyState(VK_DOWN) < 0)xAlfa = --xAlfa < 0 ? 0 : xAlfa;   // look down
    if(GetKeyState(VK_LEFT) < 0) zAlfa++;                          // look left
    if(GetKeyState(VK_RIGHT) < 0) zAlfa--;                         // look right

    float ugol = -zAlfa / 180 * M_PI;
    float speed = 0;
    if(GetKeyState('W') < 0) speed = 0.1;
    if(GetKeyState('S') < 0) speed = -0.1;
    if(GetKeyState('A') < 0) { speed = 0.1; ugol-=M_PI * 0.5;}
    if(GetKeyState('D') < 0) { speed = 0.1; ugol+=M_PI * 0.5;}
  
    // TODO: move up and down?
    if(GetKeyState('Q') < 0) { speed =  0.1; ugol*=M_PI * 0.5; }
    if(GetKeyState('E') < 0) { speed = -0.1; ugol/=M_PI * 0.5; }

    if(speed != 0){
        pos.x += sin(ugol) * speed;
        pos.y += cos(ugol) * speed;
    }
    glRotatef(-xAlfa, 1, 0, 0);
    glRotatef(-zAlfa, 0, 0, 1);
    glTranslatef(-pos.x, -pos.y, -3); // diagonal views

    printf("xAlfa: %f: \n", xAlfa);
    printf("yAlfa: %f: \n", yAlfa);
    printf("zAlfa: %f: \n", zAlfa);
    printf("pos.x: %d: \n", pos.x);
    printf("pos.y: %d: \n", pos.y);
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
                          "3D Modeling Tool",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          1920,
                          1240,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);

    glScissor(200, 100, 1024, 960);
    glViewport(200, 100, 1024, 960);
    glEnable(GL_SCISSOR_TEST);
    // glClearDepth(1.0);
    glClearColor(1.0f, 0.23f, 0.10f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    // perspectiveGL(35.0, 1.0, 0.1, 1000);
    glFrustum(-1,1, -1,1, 2, 80);
    glEnable(GL_DEPTH_TEST);

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
            glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glPushMatrix();
            MoveCamera();
            DrawGrid();
            glPopMatrix();

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

        case WM_SIZE:
            // todo: windows resize function
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

