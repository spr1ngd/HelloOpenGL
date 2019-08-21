
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

	// load textures
	Texture* texture = Texture::LoadTexture("res/texture/fur.jpg");
	Texture* secondTex = Texture::LoadTexture("res/texture/earth.bmp");
	unsigned int cubemap = Texture::LoadSkyboxTextures("res/texture/skybox/Origin/"); 

	glClearColor(41.0f / 255.0f, 71.0f / 255.0f, 121.0f / 255.0f, 1.0f);
	glViewport(0, 0, width, height);
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);   

	GPUProgram fsProgram;
	fsProgram.AttachShader(GL_VERTEX_SHADER,"res/shader/fullscreen.vs");
	fsProgram.AttachShader(GL_FRAGMENT_SHADER, "res/shader/fullscreen.fs");
	fsProgram.LinkProgram();
	fsProgram.InitializeLocation();

	GPUProgram depthRender;
	depthRender.AttachShader(GL_VERTEX_SHADER,"res/shader/fullscreen.vs");
	depthRender.AttachShader(GL_FRAGMENT_SHADER,"res/shader/depth/depthrender.fs");
	depthRender.LinkProgram();
	depthRender.InitializeLocation();

	FullScreenQuad fullscreen;
	fullscreen.Init();

	FBO fboLT;
	fboLT.AttachColorBuffer("color", GL_COLOR_ATTACHMENT0,GL_RGBA,width,height);
	fboLT.AttachDepthBuffer("depth",width,height);
	fboLT.Finish();

	FBO fboRT;
	fboRT.AttachColorBuffer("color", GL_COLOR_ATTACHMENT0, GL_RGBA, width, height);
	fboRT.AttachDepthBuffer("depth", width, height);
	fboRT.Finish();

	FBO fboLB;
	fboLB.AttachColorBuffer("color", GL_COLOR_ATTACHMENT0, GL_RGBA, width, height);
	fboLB.AttachDepthBuffer("depth", width, height);
	fboLB.Finish();

	FBO fboRB;
	fboRB.AttachColorBuffer("color", GL_COLOR_ATTACHMENT0, GL_RGBA, width, height);
	fboRB.AttachDepthBuffer("depth", width, height);
	fboRB.Finish();
	 
	//glm::vec3 cameraPos = glm::vec3(2.0f,2.0f,0.0f);
	glm::vec3 cameraPos = glm::vec3(3.0f,3.0f,0.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f,0.0f,-3.0f);
	glm::vec3 cameraDirection = glm::vec3(0.0f,1.0f,0.0f);
	// LIGHT SETTING
	float lightPos[] = {3.0f,3.0f,-3.0f,0.0f};
	float lightColor[] = {1.0f,1.0f ,1.0f,1.0f};
	float lightDirection[] = {0.0f,-1.0f,0.0f,128.0f};
	float lightIntensity = 3.0f;

	// SPOT LIGTH SETTING	
	float spotLightCutoff = 15.0f;

	// AMBIENT SETTING
	float ambientColor[] = { 0.12f,0.12f,0.12f,1.0f };

	// DIFFUSE SETTING
	float materialColor[] = {0.4f,0.4f,0.4f,1.0f};
	float diffuseColor[] = {0.9f,0.9f,0.9f,1.0f};

	// SPECULAR SETTING
	float specularColor[] = { 0.75f,0.75f,0.75f,1.0f };
	float specularMaterialColor[] = {1.0f,1.0f,1.0f,1.0f};

	// FOG SETTING
	float fogColor[] = {0.6f,0.6f,0.6f,1.0f};
	float fogDistance[] = {0.0f,30.0f,0.0f,0.0f};
	float fogDensity = 0.1f;
	float fogGradient = 2.0f;  

#pragma region draw floors 
	 
	GPUProgram receiver;
	receiver.AttachShader(GL_VERTEX_SHADER,   "res/shader/light/direction_light.vs");
	receiver.AttachShader(GL_FRAGMENT_SHADER, "res/shader/light/direction_light.fs");
	receiver.LinkProgram();
	receiver.InitializeLocation();

	receiver.DetectUniform("U_AmbientColor");
	receiver.DetectUniform("U_LightPos");
	receiver.DetectUniform("U_LightColor");
	receiver.DetectUniform("U_DiffuseColor");
	receiver.DetectUniform("U_SpecularColor");

	// load cube model
	unsigned int* indices = nullptr;
	int indexCount, vertexCount;
	VertexData* vertices = LoadObjModel("res/model/cube.obj", &indices, indexCount, vertexCount);

	GLuint receiverVAO = CreateVAO([&]()
		{
			GLuint receiverVBO = CreateBufferObject(GL_ARRAY_BUFFER, sizeof(VertexData) * vertexCount, GL_STATIC_DRAW, vertices);
			glBindBuffer(GL_ARRAY_BUFFER, receiverVBO);

			glEnableVertexAttribArray(receiver.GetLocation(VERTEX));
			glVertexAttribPointer(receiver.GetLocation(VERTEX), 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);

			glEnableVertexAttribArray(receiver.GetLocation(NORMAL));
			glVertexAttribPointer(receiver.GetLocation(NORMAL), 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(float) * 3));

			glEnableVertexAttribArray(receiver.GetLocation(TEXCOORD));
			glVertexAttribPointer(receiver.GetLocation(TEXCOORD), 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(float) * 6));

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		});
	GLuint receiverIBO = CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexCount, GL_STATIC_DRAW, indices);


	glm::mat4 receiverModel = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,-3.0f)) * 
		//glm::rotate(glm::mat4(1.0f) , 30.0f,glm::vec3(0.0f,1.0f,0.0f)) * 
		glm::scale(glm::mat4(1.0f) , glm::vec3(1.0f));
	glm::mat4 receiverView = glm::lookAt(cameraPos, cameraTarget, cameraDirection);
	glm::mat4 receiverProjection = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 200.0f);
	glm::mat4 receiverNM = glm::inverseTranspose(receiverModel);

	float receiverRotateSpeed = 3.0f;
	float receiverRotate = 0.0f;

	auto renderShadowReceiver = [&](void) 
	{
		glUseProgram(receiver.mProgram); 
		glUniformMatrix4fv(receiver.GetLocation(M_MATRIX),1,GL_FALSE,glm::value_ptr(receiverModel));
		glUniformMatrix4fv(receiver.GetLocation(V_MATRIX), 1, GL_FALSE, glm::value_ptr(receiverView));
		glUniformMatrix4fv(receiver.GetLocation(P_MATRIX), 1, GL_FALSE, glm::value_ptr(receiverProjection)); 

		glUniform4fv(receiver.GetLocation("U_AmbientColor"), 1, ambientColor);
		glUniform4fv(receiver.GetLocation("U_LightPos"), 1, lightPos);
		glUniform4fv(receiver.GetLocation("U_LightColor"),1,lightColor);
		glUniform4fv(receiver.GetLocation("U_DiffuseColor"),1,diffuseColor);	
		glUniform4fv(receiver.GetLocation("U_SpecularColor"), 1, specularColor);

		glBindVertexArray(receiverVAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,receiverIBO);
		glDrawElements(GL_TRIANGLES, indexCount,GL_UNSIGNED_INT,(void*)0);
		glBindVertexArray(0);
		glUseProgram(0);
	};

#pragma endregion

#pragma region draw shadow caster

	GPUProgram caster;
	caster.AttachShader(GL_VERTEX_SHADER, "res/shader/unlit/unlitcolor.vs");
	caster.AttachShader(GL_FRAGMENT_SHADER, "res/shader/unlit/unlitcolor.fs");
	caster.LinkProgram();
	caster.InitializeLocation(); 

	// load sphere model
	unsigned int* cubeIndices = nullptr;
	int cubeVertexCount, cubeIndexCount;
	VertexData* cubeVertices = LoadObjModel("res/model/sphere.obj", &cubeIndices, cubeIndexCount, cubeVertexCount);

	GLuint casterVAO = CreateVAO([&]()
	{
		GLuint casterVBO = CreateBufferObject(GL_ARRAY_BUFFER, sizeof(VertexData) * cubeVertexCount, GL_STATIC_DRAW, cubeVertices);
		glBindBuffer(GL_ARRAY_BUFFER, casterVBO);

		glEnableVertexAttribArray(caster.GetLocation(VERTEX));
		glVertexAttribPointer(caster.GetLocation(VERTEX), 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);

		glEnableVertexAttribArray(caster.GetLocation(NORMAL));
		glVertexAttribPointer(caster.GetLocation(NORMAL), 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(float) * 3));

		glEnableVertexAttribArray(caster.GetLocation(TEXCOORD));
		glVertexAttribPointer(caster.GetLocation(TEXCOORD), 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(float) * 6));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	});
	GLuint casterIBO = CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * cubeIndexCount, GL_STATIC_DRAW, cubeIndices);

	glm::mat4 casterModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.75f, -3.0f)) *
		//glm::rotate(glm::mat4(1.0f), 30.0f, glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(0.15f));
	glm::mat4 casterView = glm::lookAt(cameraPos,cameraTarget,cameraDirection);
	glm::mat4 casterProjection = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 200.0f);
	glm::mat4 casterNM = glm::inverseTranspose(casterModel);

	auto renderShadowCast = [&](void) 
	{
		glUseProgram(caster.mProgram);

		glUniformMatrix4fv(caster.GetLocation(M_MATRIX), 1, GL_FALSE, glm::value_ptr(casterModel));
		glUniformMatrix4fv(caster.GetLocation(V_MATRIX), 1, GL_FALSE, glm::value_ptr(casterView));
		glUniformMatrix4fv(caster.GetLocation(P_MATRIX), 1, GL_FALSE, glm::value_ptr(casterProjection));
		//glUniformMatrix4fv(caster.GetLocation(NM_MATRIX), 1, GL_FALSE, glm::value_ptr(casterNM)); 
		
		glBindVertexArray(casterVAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, casterIBO);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->mTextureID);
		glUniform1i(caster.GetLocation(MAIN_TEXTURE), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, secondTex->mTextureID);
		glUniform1i(caster.GetLocation(SECOND_TEXTURE), 1);

		glDrawElements(GL_TRIANGLES,cubeIndexCount,GL_UNSIGNED_INT,(void*)0);
		glBindVertexArray(0);
		glUseProgram(0); 
	}; 

#pragma endregion 

#pragma region multi render

	auto render2LT = [&](void) 
	{
		glDisable(GL_DEPTH_TEST);
		fboLT.Bind();
		renderShadowReceiver();
		renderShadowCast();
		fboLT.Unbind();

		glUseProgram(fsProgram.mProgram);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fboLT.GetBuffer("color"));
		glUniform1i(fsProgram.GetLocation(MAIN_TEXTURE), 0);
		fullscreen.Draw(fsProgram.GetLocation(VERTEX), fsProgram.GetLocation(TEXCOORD), new Rect(-1.0f, 0.0f, 1.0f, 0.0f));
		glUseProgram(0);
		glEnable(GL_DEPTH_TEST);
	};

	auto render2RT = [&](void) 
	{
		cameraPos = glm::vec3(0.0f, 3.0f, -3.0f);
		cameraTarget = glm::vec3(0.0f, 0.0f, -3.0f);
		cameraDirection = glm::vec3(0.0f, 0.0f, 1.0f);
		receiverView = glm::lookAt(cameraPos, cameraTarget, cameraDirection);
		casterView = glm::lookAt(cameraPos, cameraTarget, cameraDirection);
		glDisable(GL_DEPTH_TEST);
		fboRT.Bind();
		renderShadowReceiver();
		renderShadowCast();
		fboRT.Unbind(); 

		glUseProgram(depthRender.mProgram);
		glActiveTexture(GL_TEXTURE0);
		GLuint depthBuffer = fboRT.GetBuffer("depth");
		glBindTexture(GL_TEXTURE_2D,depthBuffer);
		glUniform1i(depthRender.GetLocation(MAIN_TEXTURE), 0);
		fullscreen.Draw(depthRender.GetLocation(VERTEX), depthRender.GetLocation(TEXCOORD), new Rect(0.0f, 1.0f, 1.0f, 0.0f));
		glUseProgram(0);
		glEnable(GL_DEPTH_TEST);
		cameraPos = glm::vec3(3.0f, 3.0f, 0.0f);
		cameraTarget = glm::vec3(0.0f, 0.0f, -3.0f);
		cameraDirection = glm::vec3(0.0f, 1.0f, 0.0f);
		receiverView = glm::lookAt(cameraPos, cameraTarget, cameraDirection);
		casterView = glm::lookAt(cameraPos, cameraTarget, cameraDirection);

	};

	auto render2LB = [&](void) 
	{

	};

	auto render2RB = [&](void) 
	{

	};

#pragma endregion

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FLOAT);
	glEnable(GL_DEPTH_TEST);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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

		//receiverRotate += receiverRotateSpeed;
		//if (receiverRotate > 400.0f)
		//	receiverRotate -= 360.0f;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.01f, 0.01f, 0.01f, 1.0f);   
		 
		// TODO: draw quad as shadow_receive
		renderShadowReceiver();
		// TODO: draw sphere as shadow_cast 
		renderShadowCast();
		// TODO: draw origin screen in left top window.
		// TODO: draw depth buffer in right top window.

#pragma region ∑÷∆¡‰÷»æ

		// Render To Left Top
		render2LT();

		// Render To Right Top 
		// draw the sphere's shadow received by quad.
		render2RT();

#pragma endregion

		glFinish();
		SwapBuffers(dc);
	}
	return  0;
}