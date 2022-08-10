#include <windows.h>
#include <gl/gl.h>
#include <math.h>

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);


float light;

/* Assignment 1: Draw a Village */
void Quad(float x, float y, float dx, float dy)
{
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y);
        glVertex2f(x+dx, y);
        glVertex2f(x+dx, y+dy);
        glVertex2f(x, y+dy);
    glEnd();
}

void DrawHouse(float x, float y, float size)
{

    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    glColor3f(light,0,0);
    Quad(-0.5, -0.5, 1, 0.5);
    Quad( 0.2,    0, 0.2, 0.5);
    glColor3f(light, light, light);
    Quad(-0.1, -0.3, 0.2, 0.2);

    glBegin(GL_TRIANGLES);
    glVertex2f(-0.6, 0);
    glVertex2f(0.6, 0);
    glVertex2f(0, 0.5);
    glEnd();

    glPopMatrix();
}

void DrawSun()
{
    static float alfa = 0;
    alfa -= 2;
    glPushMatrix();
        glTranslatef(0, -1, 0);
        glRotatef(alfa, 0, 0, 1);
        glTranslatef(1.7, 0, 0);
        glColor3f(1,1,0);
        DrawCircle(); //
    glPopMatrix();
    light = sin(alfa / 180 * M_PI) * 0.45 + 0.5;
}

//40
void DrawCircle()
{
    int cnt = 40;
    float x, y;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0, 0);

    float l = 0.1; // radius of the sun
    float a = M_PI * 2 / cnt;
    for(int i=-1; i<cnt; i++){
        x = sin(a * i) * l;
        y = cos(a * i) * l;
        glVertex2f(x, y);
    }
    glEnd();
}

void DrawBackground()
{
    glColor3f(0.6 * light, 0.8*light, light);
    Quad(-2, 0.15, 4, 1);
    glColor3f(0.4 * light, 0.7 *light, 0.2 * light);
    Quad(-2, 0.15, 4, -2);
}

/*Examples*/
void ShowObj(float colors [3][3], float vertices[3][2])
{
        glBegin(GL_TRIANGLES);

        glColor3f(colors[0][0], colors[0][1], colors[0][2]);
        glVertex2f(vertices[0][0], vertices[0][1]);

        glColor3f(colors[1][0], colors[1][1], colors[1][2]);
        glVertex2f(vertices[1][0], vertices[1][1]);

        glColor3f(colors[2][0], colors[2][1], colors[2][2]);
        glVertex2f(vertices[2][0], vertices[2][1]);

        glEnd();
}
void draw()
{


              /*
            glBegin(GL_TRIANGLES);
            glColor3f(1,0,0);
            glVertex2f(0,0);
            glVertex2f(0.2,0);
            glVertex2f(0,0.2);

            glColor3f(0,1,0);
            glVertex2f(-0.5,0);
            glVertex2f(-0.8,0);
            glVertex2f(-0.8, -0.3);

            glEnd();

            for(int i = 0; i < 4; i++){
                float dx = i * 0.33 - 1;
                float dy = -0.6;
                glBegin(GL_TRIANGLE_FAN);
                    glColor3f(i*1%1, i*0.33, i*0.33);
                    glVertex2f(dx, dy);
                    glVertex2f(dx, dy+0.2);
                    glVertex2f(dx+0.2, dy+0.2);
                    glVertex2f(dx+0.2, dy);
                glEnd();
            }
            */
}

void DrawSquare()
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
                          "Assignment 1: Village",
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

            DrawBackground();
            DrawHouse(-0.5, -0.5, 0.4);
            DrawHouse( 0.5, -0.3, 0.3);
            DrawHouse(-0.1,    0, 0.2);

            DrawSun();

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

