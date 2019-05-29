// HelloOpenGL.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "HelloOpenGL.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_HELLOOPENGL, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance); 
    // 执行应用程序初始化:
    /*if (!InitInstance (hInstance, nCmdShow, dc))
    {
        return FALSE;
    }*/

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	// creata opengl render context
	HDC dc = GetDC(hWnd);
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nVersion = 1;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;

	int pixelFormat = ChoosePixelFormat(dc, &pfd);
	SetPixelFormat(dc, pixelFormat, &pfd);

	HGLRC rc = wglCreateContext(dc);
	wglMakeCurrent(dc, rc);

	//opengl init
	glMatrixMode(GL_PROJECTION);
	gluPerspective(50,800.0f/600.0f,0.1f,1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0.1,0.4,0.6,1.0); // set clear color for background

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd); 
	glEnable(GL_CULL_FACE);  // ccw
	/*glPolygonMode(GL_FRONT,GL_POINT);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);*/
	//glFrontFace(GL_CCW);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HELLOOPENGL));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } 
		// draw scene
		glLoadIdentity(); // 重置为单位矩阵
		glClear(GL_COLOR_BUFFER_BIT);

		glScalef(1.0f, 1.0f,1.0f);
		glRotatef(30.0f,0.0f,0.0f,1.0f);
		glTranslatef(5.0f,0.0f,0.0f);

		glColor4ub(255,255,255,255); // set current color : white

		//glPointSize(10.0f);
		//glBegin(GL_POINTS); // start to draw something
		//glVertex3f(0.0f,0.0f,-0.5f);

		glPointSize(10.0f);
		glLineWidth(2.0f);

		// line type
		//glBegin(GL_LINES);
		//glBegin(GL_LINE_LOOP);
		//glBegin(GL_LINE_STRIP);

		// triangles front face : ccw counter clock wise
		glBegin(GL_TRIANGLES); 
		//glBegin(GL_TRIANGLE_STRIP); // 奇数点 n n+1 n+2 // 偶数点 n+1 n n+2
		//glBegin(GL_TRIANGLE_FAN);
		
		/*glColor4ub(255, 0, 0, 255); 
		glVertex3f(0.0f,0.0f,-15.0f);
		glColor4ub(0, 255, 0, 255);
		glVertex3f(5.0f, 0.0f, -15.0f);
		glColor4ub(0, 0, 255, 255);
		glVertex3f(0.0f, 5.0f, -15.0f);
		glColor4ub(255, 0, 0, 255);
		glVertex3f(-5.0f, 0.0f, -15.0f);
		glColor4ub(0, 255, 0, 255);
		glVertex3f(0.0f, 10.0f, -15.0f); */
		//glColor4ub(0, 0, 255, 255);
		//glVertex3f(5.0f, 5.0f, -15.0f);

		// quad
		//glBegin(GL_QUADS);
		//glBegin(GL_QUAD_STRIP); // n/2 -1 个四边形 // 2n-1 2n 2n+2 2n+1

		// polygon
		//glBegin(GL_POLYGON);
		glColor4ub(255, 255, 0, 255);
		glVertex3f(4.0f, 4.0f, -10.0f);
		glVertex3f(-4.0f, 4.0f, -10.0f);
		glVertex3f(-4.0f, -4.0f, -10.0f);
		/*glColor4ub(255, 255, 255, 255);
		glVertex3f(4.0f, 4.0f,-10.0f);
		glVertex3f(-4.0f, 4.0f, -10.0f);*/
	/*	glVertex3f(-4.0f, -4.0f, -10.0f);
		glVertex3f(4.0f, -4.0f, -10.0f);*/


		glEnd();// draw end

		// present scene
		SwapBuffers(dc);
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HELLOOPENGL));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_HELLOOPENGL);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
} 

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
