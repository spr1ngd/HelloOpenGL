﻿// HelloOpenGL.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "HelloOpenGL.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "texture.h"
#include "util.h"
#include "objloader.h"
#include "Camera.h" 
#include <mmsystem.h>
#include "skybox.h"
#include "ImageSprite.h"
#include "Ground.h"
#include "button.h"
#include "particle.h"
#include "fbxmodel.h"
#include "fontmanager.h"

Camera camera;
Skybox skybox;
  
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"winmm.lib")

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

float viewportWidth;
float viewportHeight;

ObjLoader objLoader;
Ground ground;
Texture* texture;
ImageSprite sprite;
Button* button;
ImageSprite fadeImage;
Particle* rootParticle;
FBXModel fbxModel;

void RenderOneFrame(float deltaTime) 
{
	camera.SwitchTo3D();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, viewportWidth, viewportHeight);
	// set up camera
	camera.Update(deltaTime); // 固定为60FPS
	glEnable(GL_TEXTURE_2D);
	skybox.Draw(camera.mPos);
	ground.Draw();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture->mTextureID);
	// mirror image
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_COLOR, GL_ONE);
	glPushMatrix();
	glTranslatef(0, -2.0f, 0);
	glRotatef(180.0f, 1, 0, 0);
	objLoader.Draw();
	glPopMatrix();
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	objLoader.Draw();
	fbxModel.Draw();
	//  draw 2d ui , switch camera to 2d mode.
	camera.SwitchTo2D();
	glLoadIdentity();
	//sprite.Draw();
	button->Draw();

	// fade image
	/*fadeImage.Update(deltaTime);
	fadeImage.Draw();*/

	// particle 
	rootParticle->Update(deltaTime);
	rootParticle->Draw();
}

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
	RECT rect;
	rect.left = 0;
	rect.right = 800;
	rect.top = 0;
	rect.bottom = 600;
	AdjustWindowRect(&rect,WS_OVERLAPPEDWINDOW,NULL);


	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		100, 100, rect.right-rect.left, rect.bottom-rect.top, nullptr, nullptr, hInstance, nullptr);

	GetClientRect(hWnd, &rect);
	viewportWidth = rect.right - rect.left;
	viewportHeight = rect.bottom - rect.top;
	// creat OpenGL render context
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
	glViewport(0,0,viewportWidth,viewportHeight);

	FontManager::Init(dc);

	// OpenGL init
	camera.mViewportWidth = viewportWidth;
	camera.mViewportHeight = viewportHeight;

	// texture load
	texture = Texture::LoadTexture("res/earth.bmp"); 

	// obj model load.

	objLoader.Init("res/Sphere.obj"); 

	// skybox load
	skybox.Init("res/skybox"); 
	
	ground.Init();

	// image sprite load
	sprite.SetTexture(Texture::LoadTexture("res/头像 男孩.png"));
	sprite.SetRect(-200.0f,-200.0f,400.0f,300.0f); 
	 
	// particle -> procedural texture.
	Texture* proceduralTexture = new Texture;
	proceduralTexture->mTextureID = GenerateProceduralTexture(256.0f);
	/*fadeImage.SetTexture(proceduralTexture);
	fadeImage.SetRect(0,0,100.0f,100.0f);*/

	Particle::InitSpeed();
	rootParticle = new Particle(true);
	rootParticle->SetTexture(proceduralTexture);
	for (int i = 0; i < 1000; i++) 
	{
		Particle* particle01 = new Particle;
		particle01->SetTexture(proceduralTexture);
		particle01->SetRect(0, 0, 20.0f, 20.0f);
		rootParticle->Push(particle01);
	} 

	ImageSprite* buttonSprite = new ImageSprite;
	buttonSprite->SetTexture(Texture::LoadTexture("res/timg.jpg"));
	button = new Button;
	button->SetDefaultSprite(buttonSprite);
	button->SetRect(-350,250,100,100);
	button->AddClickEvent([]()->void 
		{
			printf("i am a button[HEAD IMAGE]\n");
		});

	ImageSprite* button2Sprite = new ImageSprite;
	button2Sprite->SetTexture(Texture::LoadTexture("res/帮助.png"));
	Button*button2 = new Button;
	button2->SetDefaultSprite(button2Sprite);
	button2->SetRect(370, 270, 30, 30);
	button2->AddClickEvent([]()->void
		{
			printf("i am a button[HELPING]\n");
			fadeImage.FadeOut(1.0f);
		});
	button->Push(button2);

	ImageSprite* button3Sprite = new ImageSprite;
	button3Sprite->SetTexture(Texture::LoadTexture("res/背包.png"));
	Button* button3 = new Button;
	button3->SetDefaultSprite(button3Sprite);
	button3->SetRect(330, 270, 30, 30);
	button3->AddClickEvent([]()->void
		{
			printf("i am a button[PACKAGE]\n");
			fadeImage.FadeIn(1.0f);
		});
	button->Push(button3);

	Texture* screenTexture = new Texture;
	screenTexture->mTextureID = CaptureScreen(viewportWidth, viewportHeight, []()->void
		{
			//RenderOneFrame(0.0f);
		});
	sprite.SetTexture(screenTexture); 

	// FBX MODEL
	fbxModel.Init("res/fbx/tauren.fbx");

	SaveScreenPixel(viewportWidth, viewportHeight, []()->void
		{
			//RenderOneFrame(0.0f);
		}, "screenshot.bmp");

	glClearColor(0.1f,0.4f,0.6f,1.0f); // set clear color for background

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd); 
	glEnable(GL_CULL_FACE);  // ccw
	glEnable(GL_DEPTH_TEST);

	// init light
	float whiteColor[] = {1.0f,1.0f,1.0f,1.0f};
	float grayColor[] = {0.2f,0.2f, 0.2f, 1.0f};
	float lightPos[] = {0.0f,1.0f,0.0f,0.0f}; // todo : qi ci zuobiao 
	glLightfv(GL_LIGHT0,GL_AMBIENT, whiteColor);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteColor);
	glLightfv(GL_LIGHT0, GL_SPECULAR, whiteColor);
	glLightfv(GL_LIGHT0,GL_POSITION, lightPos); // direction light /  spot light /  point light

	// material 
	float blackMat[] = {0.0f,0.0f,0.0f,1.0f};
	float ambient[] = {0.1f,0.1f,0.1f,1.0f};
	float diffuse[] = {0.4f,0.4f,0.4f,1.0f};
	float specular[] = {0.9f,0.9f,0.9f,1.0f};
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT,GL_SHININESS,30.0f);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	/*glPolygonMode(GL_FRONT,GL_POINT);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);*/
	//glFrontFace(GL_CCW);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HELLOOPENGL));

    MSG msg; 
	static float sTimeSinceStartUp = timeGetTime() / 1000.0f;

	float hW = viewportWidth / 2.0f;
	float hH = viewportHeight / 2.0f;

	float vertices[] = 
	{
		-hW, -hH, 0,
		0, -hH, 0,
		0, 0, 0,
		-hW, 0, 0
	};

	float texcoords[] = 
	{
		0.0f, 0.0f,
		1.0f,0.0f,
		1.0f,1.0f,
		0.0f,1.0f
	};
	 
    // 主消息循环:
    while (/*GetMessage(&msg, nullptr, 0, 0)*/true)
    {
        /*if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } */

		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) 
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// draw scene
		glLoadIdentity(); // 重置为单位矩阵		
		float currentTime = timeGetTime() / 1000.0f;
		float timeElapse = currentTime - sTimeSinceStartUp;
		sTimeSinceStartUp = currentTime;
	/*	glEnable(GL_SCISSOR_TEST);
		glScissor(0,0,viewportWidth/2.0f,viewportHeight);*/
		RenderOneFrame(timeElapse);
		glColor4ub(255,255,0,255);
		glRasterPos2f(0, 0);
		FontManager::Draw(L"你好，OPENGL");
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

POINT originPos;
bool bRotateView = false;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	float x = LOWORD(lParam);
	float y = HIWORD(lParam);
	x = x - camera.mViewportWidth / 2.0f;
	y = camera.mViewportHeight / 2.0f - y;
    switch (message)
    {
		case WM_LBUTTONDOWN:
		{ 
			button->OnTouchBegin(x,y);
			break;
		}
		case WM_LBUTTONUP: 
		{
			button->OnTouchEnd(x, y);
			button->Repaint();
			break;
		}
	case WM_KEYDOWN:
		switch (wParam)
		{
			case  'A':
				camera.isMoveLeft = true;
				break;
			case  'D':
				camera.isMoveRight = true;
				break;
			case 'W':
				camera.isMoveForward = true;
				break;
			case 'S':
				camera.isMoveBack = true;
				break;
			case 'Q':
				camera.isLeftRotate = true;
				break;
			case 'E':
				camera.isRightRotate = true;
				break;
			default:
				break;
		}
		break;
	case WM_KEYUP:
		switch (wParam)
		{
		case  'A':
			camera.isMoveLeft = false;
			break;
		case  'D':
			camera.isMoveRight = false;
			break;
		case 'W':
			camera.isMoveForward = false;
			break;
		case 'S':
			camera.isMoveBack = false;
			break;
		case 'Q':
			camera.isLeftRotate = false;
			break;
		case 'E':
			camera.isRightRotate = false;
			break;
		default:
			break;
		}
		break;
	case WM_MOUSEMOVE:
		if (bRotateView) 
		{
			POINT mousePos;
			mousePos.x = LOWORD(lParam);
			mousePos.y = HIWORD(lParam);
			ClientToScreen(hWnd, &mousePos);
			long offsetX = mousePos.x - originPos.x;
			long offsetY = mousePos.y - originPos.y;
			float angleByUp = -(float)offsetX / 1000.0f;
			float angleByRight = -(float)offsetY / 1000.0f;
			camera.Pitch(angleByRight);
			camera.Yaw(angleByUp);
			SetCursorPos(originPos.x,originPos.y);
		}
		break;
	case WM_RBUTTONDOWN:
		originPos.x = LOWORD(lParam);
		originPos.y = HIWORD(lParam);
		ClientToScreen(hWnd, &originPos);
		SetCapture(hWnd);
		ShowCursor(false);
		bRotateView = true;
		break;
	case WM_RBUTTONUP:
		bRotateView = false;
		SetCursorPos(originPos.x,originPos.y);
		ReleaseCapture();
		ShowCursor(true);		
		break;
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
