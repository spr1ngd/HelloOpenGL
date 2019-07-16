
#include <windows.h>
#include <stdio.h> 
#include "Glm/glm.hpp"
#include "Glm/ext.hpp"
#include "glew.h"
#include "wglew.h"
#include <gl/GL.h>
#include "misc.h"
#include "model.h"
#include "fbo.h"
#include "texture.h"
#include "fullscreenquad.h"
#include "gpuprogram.h"

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
	MSG msg; 

	int width, height; 
	GetClientRect(hwnd, &rect);
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;

	// create GPU program
	GLuint program = CreateGPUProgram("res/shader/diffuse.vs", "res/shader/diffuse.fs");
	GLuint MLocation, VLocation, PLocation,NMLocation, vertexLocation, normalLocation, texcoordLocation,MainTextureLocation,SecondTextureLocation;
	vertexLocation = glGetAttribLocation(program, "vertex");
	normalLocation = glGetAttribLocation(program, "normal");
	texcoordLocation = glGetAttribLocation(program,"texcoord");
	MLocation = glGetUniformLocation(program, "M");
	VLocation = glGetUniformLocation(program, "V");
	PLocation = glGetUniformLocation(program,"P");
	NMLocation = glGetUniformLocation(program, "NM");
	MainTextureLocation = glGetUniformLocation(program,"U_MainTexture");
	SecondTextureLocation = glGetUniformLocation(program,"U_SecondTexture");

	/*GPUProgram program;
	program.AttachShader(GL_VERTEX_SHADER,"res/shader/diffuse.vs");
	program.AttachShader(GL_FRAGMENT_SHADER,"res/shader/diffuse.fs");
	program.LinkProgram();*/

	// load cube model
	unsigned int* indices = nullptr;
	int indexCount, vertexCount;
	VertexData* vertices = LoadObjModel("res/model/cube.obj", &indices, indexCount, vertexCount);
	Texture* texture = Texture::LoadTexture("res/texture/fur.jpg");
	Texture* secondTex = Texture::LoadTexture("res/texture/carbon_fiber.jpg");

	GPUProgram fsProgram; 
	fsProgram.AttachShader(GL_VERTEX_SHADER, "res/shader/fullscreen.vs");
	fsProgram.AttachShader(GL_FRAGMENT_SHADER,"res/shader/fullscreen.fs");
	fsProgram.LinkProgram();
	//GLuint ffffss = CreateGPUProgram("res/shader/fullscreen.vs", "res/shader/fullscreen.fs");

	FullScreenQuad fullscreen;
	fullscreen.Init(); 

	FBO fbo;
	fbo.AttachColorBuffer("color", GL_COLOR_ATTACHMENT0, GL_RGBA, width, height);
	fbo.AttachDepthBuffer("depth", width, height);
	fbo.Finish();

	GLuint vao = CreateVAO([&]()
	{
		GLuint vbo = CreateBufferObject(GL_ARRAY_BUFFER, sizeof(VertexData) * vertexCount, GL_STATIC_DRAW, vertices);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glEnableVertexAttribArray(vertexLocation);
		glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);
		glEnableVertexAttribArray(normalLocation);
		glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(float) * 3));
		glEnableVertexAttribArray(texcoordLocation);
		glVertexAttribPointer(texcoordLocation,2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(float) * 6));
		glBindBuffer(GL_ARRAY_BUFFER,0);
	});
	GLuint ibo = CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER,sizeof(unsigned int)*indexCount,GL_STATIC_DRAW,indices);

	glClearColor(41.0f / 255.0f, 71.0f / 255.0f, 121.0f / 255.0f, 1.0f);
	glViewport(0, 0, width, height);
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd); 

	glm::mat4 MODEL = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -4.0f)) * glm::rotate(glm::mat4(1.0f),60.0f,glm::vec3(0.0f,1.0f,0.0f));
	glm::mat4 PROJECTION = glm::perspective(45.0f, float(width) / (float)height, 0.1f, 200.0f);
	glm::mat4 ORTHO = glm::ortho(-400.0f, 400.0f, -300.0f, 300.0f);
	glm::mat4 VIEW = glm::mat4(1.0f);
	glm::mat4 NM = glm::inverseTranspose(MODEL);

	auto render = [&](void)
	{
		glUseProgram(program);
		glUniformMatrix4fv(MLocation, 1, GL_FALSE, glm::value_ptr(MODEL));
		glUniformMatrix4fv(VLocation, 1, GL_FALSE, glm::value_ptr(VIEW));
		glUniformMatrix4fv(PLocation, 1, GL_FALSE, glm::value_ptr(PROJECTION));
		glUniformMatrix4fv(NMLocation, 1, GL_FALSE,glm::value_ptr(NM));

		// bind ibo
		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,texture->mTextureID);
		glUniform1i(MainTextureLocation,0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, secondTex->mTextureID);
		glUniform1i(SecondTextureLocation,1);
 
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, (void*)0);
		glBindVertexArray(0);
		glUseProgram(0);
		glFinish();
	};
 
	auto fsRender = [&](void) 
	{
		glUseProgram(fsProgram.mProgram);
		glBindBuffer(GL_ARRAY_BUFFER,fullscreen.mVBO);

		glDisable(GL_DEPTH_TEST);

		glActiveTexture(GL_TEXTURE0);
		GLuint cb = fbo.GetBuffer("color");
		glBindTexture(GL_TEXTURE_2D,cb);
		glUniform1i(fsProgram.GetLocation(MAIN_TEXTURE), 0);
		fullscreen.Draw(fsProgram.GetLocation(VERTEX),fsProgram.GetLocation(TEXCOORD));
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glUseProgram(0);
	};

	glEnable(GL_CULL_FACE); 
	//glEnable(GL_DEPTH_TEST); 

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
		fbo.Bind();
		glClearColor(0.6f, 0.8f, 0.2f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		render();
		fbo.Unbind();

		glClearColor(41.0f / 255.0f, 71.0f / 255.0f, 121.0f / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		fsRender();
		glFinish();
		SwapBuffers(dc);
	}
	return  0;
}