
#include <windows.h>
#include <stdio.h>
#include "Glm/glm.hpp"
#include "Glm/ext.hpp"
#include "util.h"
#include "misc.h"
#include "model.h"
#include "glew.h"
#include <gl/GL.h>
#include "texture.h"

#pragma comment (lib,"glew32.lib")
#pragma comment (lib,"opengl32.lib")
//#define GLEW_STATIC

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

	HWND hwnd = CreateWindowEx(NULL,"OpenGL","RenderWindow",WS_OVERLAPPEDWINDOW,100,100,800,600,NULL,NULL,hInstance,NULL);

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

	// ´´½¨GPU PROGRAM
	GLuint program = CreateGPUProgram("res/shader/ui.vs","res/shader/ui.fs");
	if (program == 0)
	{
		return 0;
	}
	GLint MLocation, VLocation, PLocation,NMLocation, posLocation, normalLocation ,texcoordLocation,MainText_Location;
	MLocation = glGetUniformLocation(program, "M");
	VLocation = glGetUniformLocation(program, "V");
	PLocation = glGetUniformLocation(program, "P");
	NMLocation = glGetUniformLocation(program,"NM");
	posLocation = glGetAttribLocation(program, "vertex");
	normalLocation = glGetAttribLocation(program,"normal");
	texcoordLocation = glGetAttribLocation(program,"texcoord");
	MainText_Location = glGetUniformLocation(program,"U_MainTexture");

	unsigned int* indices = nullptr;
	int indexCount = 0, vertexCount = 0;
	VertexData* vertices = LoadObjModel("res/model/Quad.obj", &indices, indexCount, vertexCount);
	Texture* texture = Texture::LoadTexture("res/texture/Í·Ïñ ÄÐº¢.png");

	GLuint vbo = CreateBufferObject(GL_ARRAY_BUFFER, sizeof(VertexData)* vertexCount, GL_STATIC_DRAW, vertices);
	GLuint ibo = CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexCount, GL_STATIC_DRAW, indices);
	printf("vertex count %d,index count %d",vertexCount,indexCount);

	glClearColor(41.0f / 255.0f, 71.0f / 255.0f, 121.0f / 255.0f, 1.0f);
	// glClearColor(0.0f,0.0f,0.0f,1.0f);
	glViewport(0, 0, 800, 600);
	glEnable(GL_DEPTH_TEST);
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	float identity[] =
	{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
	 
	glm::mat4 modelMat =
		//glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, 5.0f, 0.0f))
		//;// *glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f))
	    /***/glm::scale(glm::mat4(1.0f), glm::vec3(100.0f, 100.0f, 1.0f));
	glm::mat4 projection = glm::perspective(45.0f, 800.0f/600.0f, 0.1f, 1000.0f);
	glm::mat4 uiMatrix = glm::ortho(-400.0f,400.0f,-300.0f,300.0f);
	glm::mat4 normalMatrix = glm::inverseTranspose(modelMat);

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA );

	float angle = 0.0f;
	MSG msg;
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

		/*angle += 0.005f;
		if (angle >= 3.1415926f*2)
			angle -= 3.1415926f*2; 

		modelMat =
			glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f)) *
			glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
		normalMatrix = glm::inverseTranspose(modelMat);*/

		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		glUseProgram(program); 
		glUniformMatrix4fv(MLocation, 1, GL_FALSE, glm::value_ptr(modelMat));
		glUniformMatrix4fv(VLocation, 1, GL_FALSE, identity);
		glUniformMatrix4fv(PLocation, 1, GL_FALSE, glm::value_ptr(uiMatrix));
		glUniformMatrix4fv(NMLocation,1, GL_FALSE, glm::value_ptr(normalMatrix));
		glBindTexture(GL_TEXTURE_2D, texture->mTextureID);
		glUniform1i(MainText_Location, 0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glEnableVertexAttribArray(posLocation);
		glVertexAttribPointer(posLocation, 3, GL_FLOAT,GL_FALSE, sizeof(VertexData),(void*)0);

		glEnableVertexAttribArray(normalLocation);
		glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(float) * 3));

		glEnableVertexAttribArray(texcoordLocation);
		glVertexAttribPointer(texcoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(float) * 6));
		  
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, (void*)0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

		glUseProgram(0);

		SwapBuffers(dc);
	}
	return 0;
}