#include <windows.h>
#include <gl/gl.h>

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

#define MapW 10
#define MapH 10

typedef struct{
    BOOL mine, flag, open;
    int cntAround;
}TCell;

TCell map[MapW][MapH];
int mines;
int closedCell;
BOOL failed;

BOOL IsCellInMap(int x, int y)
{
    return (x>=0) && (y>=0) && (x<MapW) && (y<MapH);
}

void ScreenToOpenGL(HWND hwnd, int x, int y, float *ox, float *oy)
{
    RECT rect;
    GetClientRect(hwnd, &rect);
    *ox = x / (float)rect.right * MapW;
    *oy = MapH - y / (float)rect.bottom * MapH;
}

void GameNew()
{
    srand(time(NULL));
    memset(map, 0, sizeof(map));
    mines=20;
    closedCell = MapW*MapH;
    failed = FALSE;
    for(int i=0; i<mines; i++){
        int x = rand() % MapW;
        int y = rand() & MapH;
        if(map[x][y].mine)
            i--;
        else{
            map[x][y].mine = TRUE;
            for(int dx=-1; dx<2; dx++){
                for(int dy=-1; dy<2; dy++){
                    if(IsCellInMap(x+dx, y+dy))
                        map[x+dx][y+dy].cntAround += 1;
                }
            }
        }
    }
}

void ShowCount(int a)
{
    void Line(float x1, float y1, float x2, float y2)
    {
        glVertex2f(x1,y1);
        glVertex2f(x2,y2);
    }
    glLineWidth(3);
    glColor3f(1,1,0);
    glBegin(GL_LINES);
        if((a!=1) && (a!=4)) Line(0.3, 0.85, 0.7, 0.85);
        if((a!=0) && (a!=1) && (a!=7)) Line(0.3, 0.5, 0.7, 0.5);
        if((a!=1) && (a!=4) && (a!=7)) Line(0.3, 0.16, 0.7, 0.15);

        if((a!=5) && (a!=6)) Line(0.7, 0.5, 0.7, 0.85);
        if((a!=2)) Line(0.7, 0.5, 0.7, 0.15);

        if((a!=1) && (a!=2) && (a!=3) && (a!=7)) Line(0.4, 0.5, 0.4, 0.85);
        if((a==0) || (a==2) || (a==6) || (a==8) ) Line(0.3, 0.5, 0.4, 0.15);
    glEnd();
}

void ShowMine()
{
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0,0,0);
    glVertex2f(0.3, 0.3);
    glVertex2f(0.7, 0.3);
    glVertex2f(0.7, 0.7);
    glVertex2f(0.3, 0.7);
    glEnd();
}

void ShowField()
{
    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(0.8,0.8,0.3); glVertex2f(0, 1);
    glColor3f(0.3,0.6,0.3); glVertex2f(1, 1); glVertex2f(0,0);
    glColor3f(0.3,0.5,0.3); glVertex2f(1, 0);
    glEnd();
}

void ShowFieldOpen()
{
    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(0.3,0.7,0.8); glVertex2f(0, 1);
    glColor3f(0.7,0.7,0.7); glVertex2f(1, 1); glVertex2f(0,0);
    glColor3f(0.6,0.6,0.6); glVertex2f(1, 0);
    glEnd();
}

void ShowFlag()
{
    //drawing the actual flag
    glBegin(GL_TRIANGLES);
        glColor3f(1,0,0);
        glVertex2f(0.25, 0.75);
        glVertex2f(0.85, 0.5);
        glVertex2f(0.25, 0.25);
    glEnd();

    glLineWidth(5);
    glBegin(GL_LINES);
        glColor3f(0,0,0);
        glVertex2f(0.25, 0.75);
        glVertex2f(0.25, 0);
    glEnd();
}

// this will explore all close by fields and open them
void OpenFields(int x, int y)
{
    if(!IsCellInMap(x, y)||map[x][y].open) return;
    map[x][y].open=TRUE;
    if(map[x][y].cntAround == 0){
        for(int dx=-1; dx<2; dx++){
            for(int dy=-1; dy<2; dy++){
                OpenFields(x+dx, y+dy);
            }
        }
    }
    //case user click on a field and it exploded.
    // then explore all fields.
    if(map[x][y].mine){
        failed = TRUE;
        for(int j=0; j< MapH; j++){
            for(int i=0; i<MapW; i++){
                map[i][j].open = TRUE;
            }
        }
    }
}

void GameShow()
{
    glLoadIdentity();
    glScalef(2.0/MapW, 2.0/MapH, 1);
    //position element at bottom left of the window.
    glTranslatef(-MapW*0.5, -MapH*0.5, 0);

    for(int j =0;j<MapH; j++){
        for(int i=0; i<MapW; i++){
            glPushMatrix();
            glTranslatef(i, j, 0);
            if(map[i][j].open){
                ShowFieldOpen();
                if(map[i][j].mine)
                    ShowMine();
                else if(map[i][j].cntAround > 0)
                    ShowCount(map[i][j].cntAround);
            }
            else {
                ShowField();
                if(map[i][j].flag) ShowFlag();
            }
            glPopMatrix();
        }
    }
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

    GameNew();

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
            GameShow();
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

        // Left Mouse Button Click
        case WM_LBUTTONDOWN:
        {
            POINTFLOAT pf;
            ScreenToOpenGL(hwnd, LOWORD(lParam), HIWORD(lParam),
                           &pf.x, &pf.y);
            int x = (int)pf.x;
            int y = (int)pf.y;
            // checking !map[x][y].flag
            // this wont allow you to explore the flagged area/
            if(IsCellInMap(x, y) && !map[x][y].flag)
                    OpenFields(x,y );

                    //map[x][y].open = TRUE;
        }break;

        // Right Mouse Button Click
        case WM_RBUTTONDOWN:
        {
            POINTFLOAT pf;
            ScreenToOpenGL(hwnd, LOWORD(lParam), HIWORD(lParam),
                           &pf.x, &pf.y);
            int x = (int)pf.x;
            int y = (int)pf.y;
            if(IsCellInMap(x, y))
                    map[x][y].flag = !map[x][y].flag;
        }break;

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

