
#include <windows.h>
#include <stdio.h>
#include "Glm/glm.hpp"
#include "Glm/ext.hpp"
#include "util.h"
#include "misc.h"
#include "model.h"
#include "glew.h"
#include "wglew.h"
#include <gl/GL.h>
#include "texture.h"
#include "timer.h"
#include "frustum.h"

#pragma comment (lib,"glew32.lib")
#pragma comment (lib,"opengl32.lib")
//#define GLEW_STATIC

HGLRC CreateARBWindow(HDC dc)
{
	HGLRC rc = nullptr;
	GLint attribs[] = 
	{
		WGL_DRAW_TO_WINDOW_ARB,GL_TRUE,
		WGL_ACCELERATION_ARB,WGL_FULL_ACCELERATION_ARB,
		WGL_DOUBLE_BUFFER_ARB,GL_TRUE,
		WGL_RED_BITS_ARB,8,
		WGL_GREEN_BITS_ARB,8,
		WGL_BLUE_BITS_ARB,8,
		WGL_ALPHA_BITS_ARB,8,
		WGL_DEPTH_BITS_ARB,24,
		WGL_STENCIL_BITS_ARB,8,
		WGL_SAMPLE_BUFFERS_ARB,GL_TRUE,
		WGL_SAMPLES_ARB,16,
		NULL,NULL
	};
	int pixelFormat[256]={0};
	UINT formatNum = 0;
	wglChoosePixelFormatARB(dc, attribs, NULL, 256, pixelFormat, &formatNum);
	printf("support format number is %u\n", formatNum);
	if (formatNum > 0)
	{
		PIXELFORMATDESCRIPTOR pfd;
		DescribePixelFormat(dc,pixelFormat[0],sizeof(pfd),&pfd);
		SetPixelFormat(dc, pixelFormat[0], &pfd);

		int contextAttributes[] = 
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB,3.1,
			WGL_CONTEXT_MINOR_VERSION_ARB,3,
			WGL_CONTEXT_PROFILE_MASK_ARB,WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
			0
		};
		rc = wglCreateContextAttribsARB(dc,nullptr,contextAttributes);
	}
	return rc;
}

LRESULT CALLBACK GLWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR lpCmdLine,_In_ int nCmdShow) 
{
	WNDCLASSEX wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = NULL;
	wndClass.hCursor = LoadCursor(NULL,IDC_ARROW);
	wndClass.hIcon = NULL;
	wndClass.hIconSm = NULL;
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = GLWindowProc;
	wndClass.lpszClassName = "OpenGL";
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_VREDRAW | CS_HREDRAW;
	ATOM atom = RegisterClassEx(&wndClass);
	RECT rect;
	rect.left = (long)0.0;
	rect.right = (long)800.0;
	rect.top = (long)0.0;
	rect.bottom = (long)600.0;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	HWND hwnd = CreateWindowEx(NULL,"OpenGL","RenderWindow",WS_OVERLAPPEDWINDOW,100,100,rect.right-rect.left,rect.bottom-rect.top,NULL,NULL,hInstance,NULL);

	HDC dc = GetDC(hwnd);
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd,0,sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_TYPE_RGBA | PFD_DOUBLEBUFFER;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;

	int pixelFormatID = ChoosePixelFormat(dc,&pfd);
	SetPixelFormat(dc,pixelFormatID,&pfd);

	HGLRC rc = wglCreateContext(dc);
	wglMakeCurrent(dc,rc);
	glewInit();
	//if (wglChoosePixelFormatARB) 
	//{
	//	// destroy window
	//	wglMakeCurrent(dc,nullptr);
	//	wglDeleteContext(rc);
	//	rc = nullptr;
	//	ReleaseDC(hwnd,dc);
	//	dc = nullptr;
	//	DestroyWindow(hwnd);
	//	hwnd = CreateWindowEx(NULL, "OpenGL", "RenderWindow", WS_OVERLAPPEDWINDOW, 100, 100, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, NULL);
	//	rc = CreateARBWindow(GetDC(hwnd));
	//	wglMakeCurrent(dc, rc);
	//}
	MSG msg; 

	int width, height; 
	GetClientRect(hwnd, &rect);
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;

	float identity[] =
	{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
	glm::mat4 modelMat =
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, -2.0f))
		* glm::rotate(glm::mat4(1.0f), -90.0f, glm::vec3(0.0f, 1.0f, 0.0f))
		* glm::scale(glm::mat4(1.0f), glm::vec3(0.01f, 0.01f, 0.01f));
	glm::mat4 projection = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
	glm::mat4 uiMatrix = glm::ortho(-400.0f, 400.0f, -300.0f, 300.0f);
	glm::mat4 normalMatrix = glm::inverseTranspose(modelMat);
	 
	GLuint program = CreateGPUProgram("res/shader/diffuse.vs","res/shader/diffuse.fs");  
	GLint MLocation, VLocation, PLocation,NMLocation, posLocation, normalLocation ,texcoordLocation,MainText_Location,offsetLocation,surfaceColorLocation;
	MLocation = glGetUniformLocation(program, "M");
	VLocation = glGetUniformLocation(program, "V");
	PLocation = glGetUniformLocation(program, "P");
	NMLocation = glGetUniformLocation(program,"NM");
	posLocation = glGetAttribLocation(program, "vertex");
	normalLocation = glGetAttribLocation(program,"normal");
	texcoordLocation = glGetAttribLocation(program,"texcoord");
	offsetLocation = glGetAttribLocation(program, "offset");
	MainText_Location = glGetUniformLocation(program,"U_MainTexture");
	surfaceColorLocation = glGetSubroutineUniformLocation(program,GL_FRAGMENT_SHADER,"U_SurfaceColor");
	GLuint ambientShader = glGetSubroutineIndex(program,GL_FRAGMENT_SHADER,"Ambient");
	GLuint diffuseShader = glGetSubroutineIndex(program,GL_FRAGMENT_SHADER,"Diffuse");
	GLuint specularShader = glGetSubroutineIndex(program,GL_FRAGMENT_SHADER,"Specular"); 

	unsigned int* indices = nullptr;
	int indexCount = 0, vertexCount = 0; 
	VertexData* vertices = LoadObjModel("res/model/niutou.obj", &indices, indexCount, vertexCount);
	Texture* texture = Texture::LoadTexture("res/texture/niutou.bmp");  

	float offsets[] =
	{
		-1.0f,-0.5f,-4.0f,
		0.0f,-0.5f,-4.0f,
		1.0f,-0.5f,-4.0f,
	}; 
	GLuint offsetVBO = CreateBufferObject(GL_ARRAY_BUFFER, sizeof(float) * 9, GL_STATIC_DRAW, offsets);
	GLuint vao = CreateVAO([&]()
	{
		GLuint vbo = CreateBufferObject(GL_ARRAY_BUFFER, sizeof(VertexData) * vertexCount, GL_STATIC_DRAW, vertices); 
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glEnableVertexAttribArray(posLocation);
		glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);
		glEnableVertexAttribArray(normalLocation);
		glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(float) * 3));
		glEnableVertexAttribArray(texcoordLocation);
		glVertexAttribPointer(texcoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(float) * 6));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, offsetVBO);
		glEnableVertexAttribArray(offsetLocation);
		glVertexAttribPointer(offsetLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glVertexAttribDivisor(offsetLocation, 1);
	}); 

	GLuint ibo = CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexCount, GL_STATIC_DRAW, indices);
	GLuint colorBuffer, depthBuffer;
	GLuint fbo = CreateFrameBufferObject(colorBuffer, depthBuffer, width, height); 

	// È«ÆÁQUAD
	GLuint fsprogram = CreateGPUProgram("res/shader/fullscreen.vs", "res/shader/depth.fs");
	GLint fsposLocation, fsnormalLocation, fstexcoordLocation, fsMainText_Location;
	fsposLocation = glGetAttribLocation(fsprogram, "vertex");
	fsnormalLocation = glGetAttribLocation(fsprogram, "normal");
	fstexcoordLocation = glGetAttribLocation(fsprogram, "texcoord");
	fsMainText_Location = glGetUniformLocation(fsprogram, "U_MainTexture");

	unsigned int* fsindices = nullptr;
	int fsindexCount = 0, fsvertexCount = 0;
	VertexData* fsvertices = LoadObjModel("res/model/Quad.obj", &fsindices, fsindexCount, fsvertexCount); 
	 
	GLuint fsvao = CreateVAO([&]()
	{
		GLuint fsvbo = CreateBufferObject(GL_ARRAY_BUFFER, sizeof(VertexData) * fsvertexCount, GL_STATIC_DRAW, fsvertices);
		glBindBuffer(GL_ARRAY_BUFFER, fsvbo);
		glEnableVertexAttribArray(fsposLocation);
		glVertexAttribPointer(fsposLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);
		glEnableVertexAttribArray(fsnormalLocation);
		glVertexAttribPointer(fsnormalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(float) * 3));
		glEnableVertexAttribArray(fstexcoordLocation);
		glVertexAttribPointer(fstexcoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(float) * 6)); 
	});

	GLuint fsibo = CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * fsindexCount, GL_STATIC_DRAW, fsindices);
	GLuint fscolorBuffer, fsdepthBuffer;
	GLuint fsfbo = CreateFrameBufferObject(fscolorBuffer, fsdepthBuffer, width, height);

	glClearColor(41.0f / 255.0f, 71.0f / 255.0f, 121.0f / 255.0f, 1.0f);
	glViewport(0, 0, width, height); 
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	Frustum frustum;
	frustum.InitProgram();
	//frustum.InitPerspective(45.0f, (float)width / (float)height, 1, 20.0f);
	frustum.InitOrtho(-5.0f,5.0f,5.0f,-5.0f,0.5f,100.0f);

	//glEnable(GL_POINT_SPRITE);
	//glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	auto wath = [&](void)
	{
		//frustum.Draw(glm::value_ptr(modelMat), identity,glm::value_ptr(projection));
		glUseProgram(program);
		glUniformMatrix4fv(MLocation, 1, GL_FALSE, glm::value_ptr(modelMat));
		glUniformMatrix4fv(VLocation, 1, GL_FALSE, identity);
		glUniformMatrix4fv(PLocation, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(NMLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));
		glBindTexture(GL_TEXTURE_2D, texture->mTextureID);
		glUniform1i(MainText_Location, 0);

		glBindVertexArray(vao); 
		glUniformMatrix4fv(MLocation, 1, GL_FALSE, glm::value_ptr(modelMat));
		//glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &diffuseShader);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, (void*)0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 
		glBindVertexArray(0);
		glUseProgram(0);
	}; 

	auto fullscreen = [&](void)
	{  
		glUseProgram(fsprogram);
		glBindTexture(GL_TEXTURE_2D,depthBuffer);
		glUniform1i(fsMainText_Location, 0);

		glBindVertexArray(fsvao);  
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fsibo);
		glDrawElements(GL_TRIANGLES, fsindexCount, GL_UNSIGNED_INT, (void*)0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	};

	while (true) 
	{
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) 
		{
			if (msg.message == WM_QUIT) 
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}  
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//wath();

		//glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		wath(); 
		//glBindFramebuffer(GL_FRAMEBUFFER,0);

		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//fullscreen();
		SwapBuffers(dc);
	}
	return  0;
}