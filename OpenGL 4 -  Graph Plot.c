#include <windows.h>
#include <gl/gl.h>
#include <math.h>

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

POINTFLOAT *mas = NULL;
int cnt;

float scaleY = 1;

// draw the graph to plot on
void DrawLines(float alfa)
{
    static float d = 0.05;
    glPushMatrix();
    glRotatef(alfa, 0, 0, 1);
    glBegin(GL_LINES);
        glVertex2f(-1, 0);
        glVertex2f(1, 0);

        glVertex2f(1, 0);
        glVertex2f(1-d, 0+d);

        glVertex2f(1, 0);
        glVertex2f(1-d, 0-d);
    glEnd();
    glPopMatrix();
}

// Connect the dots.
void Init(float start, float finish, int count)
{
    cnt = count;
    mas = realloc(mas, sizeof(*mas) * cnt);
    float dx = (finish - start) / (cnt - 1);
    for(int i =0; i<cnt; i++){
        mas[i].x = start;
        mas[i].y = sin(start);
        start += dx;
    }
}

void Show(float scaleY)
{
    /* Amplitute of the graph function correctly. */
    float sx = 2.0 / (mas[cnt-1].x - mas[0].x);
    float dx = (mas[cnt-1].x, mas[0].x) * 0.5;
    glPushMatrix();
    glScalef(sx, 1, 1);
    glTranslatef(-dx, 0, 0);

    glBegin(GL_LINE_STRIP);
    for(int i=0; i<cnt; i++)
        glVertex2f(mas[i].x, mas[i].y);

    glPopMatrix();
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
                          "Plot Graph Function",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          1200,
                          700,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);

    static int a = 0;

    Init(-1, 10, 100);
    //Init(0, 5, 100);


    /* progra main loop */
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
            glClearColor(0.0f, 0.0f, 0.4f, 0.0f); // blueish
            glClear(GL_COLOR_BUFFER_BIT);

            glLoadIdentity();

            glLineWidth(2);
            glColor3f(1, 0, 0);
            DrawLines(0);
            glColor3f(0, 1, 0);
            DrawLines(90);

            glColor3f(0,1 ,1);
            Show(30);

            SwapBuffers(hDC);
            Sleep(1);
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

        case WM_MOUSEWHEEL:
            if((int)wParam > 0)
                scaleY *=1.5;
            else
                scaleY *= 0.7;
            if(scaleY < 0.02) scaleY = 0.02;
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

