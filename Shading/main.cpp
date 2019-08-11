
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
	GLuint program = CreateGPUProgram("res/shader/fog/fog.vs", 
									  "res/shader/fog/fog_exp2.fs");
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

	GLuint viewPosLocation = glGetUniformLocation(program,"U_ViewPos");
	GLuint lightPosLocation = glGetUniformLocation(program, "U_LightPos");
	GLuint lightColorLocation = glGetUniformLocation(program,"U_LightColor");
	GLuint lightDirectionLocation = glGetUniformLocation(program,"U_LightDirection");
	GLuint spotLightCutoffLocation = glGetUniformLocation(program,"U_SpotLightCutoff");
	GLuint lightIntensityLocation = glGetUniformLocation(program,"U_LightIntensity");
	GLuint diffuseColorLocation = glGetUniformLocation(program,"U_DiffuseColor");
	GLuint diffuseMaterialLocation = glGetUniformLocation(program,"U_DiffuseMaterial");
	GLuint specularColorLocation = glGetUniformLocation(program,"U_SpecularColor");
	GLuint specularMaterialLocation = glGetUniformLocation(program,"U_SpecularMaterial");
	GLuint ambientColorLocation = glGetUniformLocation(program,"U_AmbientColor");

	GLuint fogColorLocation = glGetUniformLocation(program, "U_FogColor");
	GLuint fogDistanceLocation = glGetUniformLocation(program,"U_FogDistance");
	GLuint fogDensityLocation = glGetUniformLocation(program,"U_FogDensity");
	GLuint fogGradientLocation = glGetUniformLocation(program,"U_FogGradient");
	

	// load cube model
	unsigned int* indices = nullptr;
	int indexCount, vertexCount;
	VertexData* vertices = LoadObjModel("res/model/cube.obj", &indices, indexCount, vertexCount);
	Texture* texture = Texture::LoadTexture("res/texture/flower.jpg");
	Texture* secondTex = Texture::LoadTexture("res/texture/earth.bmp");
	unsigned int cubemap = Texture::LoadSkyboxTextures("res/texture/skybox/");

	GPUProgram fsProgram; 
	fsProgram.AttachShader(GL_VERTEX_SHADER, "res/shader/fullscreen.vs");
	fsProgram.AttachShader(GL_FRAGMENT_SHADER,"res/shader/fullscreen.fs");
	fsProgram.LinkProgram();
	fsProgram.InitializeLocation();

	GPUProgram TLProgram;
	TLProgram.AttachShader(GL_VERTEX_SHADER,"res/shader/fullscreen.vs");
	TLProgram.AttachShader(GL_FRAGMENT_SHADER,"res/shader/image/smoothness.fs");
	TLProgram.LinkProgram();
	TLProgram.InitializeLocation();

	GPUProgram TRProgram;
	TRProgram.AttachShader(GL_VERTEX_SHADER, "res/shader/fullscreen.vs");
	TRProgram.AttachShader(GL_FRAGMENT_SHADER, "res/shader/image/edge_detect.fs");
	TRProgram.LinkProgram();
	TRProgram.InitializeLocation();

	GPUProgram BLProgram;
	BLProgram.AttachShader(GL_VERTEX_SHADER, "res/shader/fullscreen.vs");
	BLProgram.AttachShader(GL_FRAGMENT_SHADER, "res/shader/image/sharpen.fs");
	BLProgram.LinkProgram();
	BLProgram.InitializeLocation();

	GPUProgram BRProgram;
	BRProgram.AttachShader(GL_VERTEX_SHADER, "res/shader/fullscreen.vs");
	BRProgram.AttachShader(GL_FRAGMENT_SHADER, "res/shader/fullscreen.fs");
	BRProgram.LinkProgram();
	BRProgram.InitializeLocation();

	GPUProgram skyboxProgram;
	skyboxProgram.AttachShader(GL_VERTEX_SHADER,"res/shader/skybox/skybox.vs");
	skyboxProgram.AttachShader(GL_FRAGMENT_SHADER,"res/shader/skybox/skybox.fs");
	skyboxProgram.LinkProgram();
	skyboxProgram.InitializeLocation();
	skyboxProgram.DetectUniform("U_EyePos");

	FullScreenQuad fullscreen;
	fullscreen.Init(); 

	FBO fbo;
	fbo.AttachColorBuffer("color", GL_COLOR_ATTACHMENT0, GL_RGBA, width, height);
	fbo.AttachDepthBuffer("depth", width, height);
	fbo.Finish();

	FBO fbo1;
	fbo1.AttachColorBuffer("color", GL_COLOR_ATTACHMENT0, GL_RGBA, width, height);
	fbo1.AttachDepthBuffer("depth", width, height);
	fbo1.Finish();

	FBO fbo2;
	fbo2.AttachColorBuffer("color", GL_COLOR_ATTACHMENT0, GL_RGBA, width, height);
	fbo2.AttachDepthBuffer("depth", width, height);
	fbo2.Finish(); 

	FBO fbo3;
	fbo3.AttachColorBuffer("color", GL_COLOR_ATTACHMENT0, GL_RGBA, width, height);
	fbo3.AttachDepthBuffer("depth", width, height);
	fbo3.Finish();

	FBO hdrFBO;
	hdrFBO.AttachColorBuffer("color",GL_COLOR_ATTACHMENT0,GL_RGBA,width,height);
	hdrFBO.AttachColorBuffer("hdrColor",GL_COLOR_ATTACHMENT1,GL_RGBA16F,width,height);
	hdrFBO.AttachDepthBuffer("depth",width,height);
	hdrFBO.Finish();

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
	/*	program.EnableVertexAttrib(VERTEX, 3, sizeof(VertexData), (void*)0);
		program.EnableVertexAttrib(NORMAL,3,sizeof(VertexData),(void*)(sizeof(float) *3));
		program.EnableVertexAttrib(TEXCOORD,2,sizeof(VertexData),(void*)(sizeof(float) * 6));*/
		glBindBuffer(GL_ARRAY_BUFFER,0);
	});
	GLuint ibo = CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER,sizeof(unsigned int)*indexCount,GL_STATIC_DRAW,indices);

	glClearColor(41.0f / 255.0f, 71.0f / 255.0f, 121.0f / 255.0f, 1.0f);
	glViewport(0, 0, width, height);
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd); 

	glm::mat4 MODEL0 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f)) * glm::rotate(glm::mat4(1.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 MODEL1 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -9.0f)) * glm::rotate(glm::mat4(1.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 MODEL2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -15.0f)) * glm::rotate(glm::mat4(1.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		//*glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f));
	glm::mat4 PROJECTION = glm::perspective(45.0f, float(width) / (float)height, 0.1f, 200.0f);
	glm::mat4 ORTHO = glm::ortho(-width/2.0f, width/2.0f, -height/2.0f, height/2.0f);
	//glm::mat4 VIEW = glm::mat4(1.0f);
	
	glm::mat4 NM = glm::inverseTranspose(MODEL0);

	float viewPos[] = {3.0f,2.0f,2.0f};
	glm::mat4 VIEW = glm::lookAt(glm::vec3(3.0f, 2.0f, 2.0f), glm::vec3(0.0f,0.0f,-3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	// LIGHT SETTING
	float lightPos[] = {2.0f,2.0f,3.0f,0.0f};
	// float lightPos[] = {0.0f,5.0f,-2.0f,1.0f};
	float lightColor[] = {1.0f,1.0f ,1.0f,1.0f};
	float lightDirection[] = {0.0f,-1.0f,0.0f,128.0f};
	float lightIntensity = 3.0f;

	// SPOT LIGTH SETTING	
	float spotLightCutoff = 15.0f;

	// AMBIENT SETTING
	float ambientColor[] = { 0.12f,0.12f,0.12f,1.0f };

	// DIFFUSE SETTING
	float materialColor[] = {0.4f,0.4f,0.4f,1.0f};
	float diffuseColor[] = {0.2f,0.8f,0.9f,1.0f};

	// SPECULAR SETTING
	float specularColor[] = { 0.75f,0.75f,0.75f,1.0f };
	float specularMaterialColor[] = {1.0f,1.0f,1.0f,1.0f};

	// FOG SETTING
	float fogColor[] = {0.6f,0.6f,0.6f,1.0f};
	float fogDistance[] = {0.0f,30.0f,0.0f,0.0f};
	float fogDensity = 0.1f;
	float fogGradient = 2.0f;

	auto render = [&](void)
	{ 
		glUseProgram(program);
		glUniformMatrix4fv(MLocation, 1, GL_FALSE, glm::value_ptr(MODEL0));
		glUniformMatrix4fv(VLocation, 1, GL_FALSE, glm::value_ptr(VIEW));
		glUniformMatrix4fv(PLocation, 1, GL_FALSE, glm::value_ptr(PROJECTION));
		glUniformMatrix4fv(NMLocation, 1, GL_FALSE,glm::value_ptr(NM));

		glUniform4fv(ambientColorLocation,1,ambientColor);

		glUniform3fv(viewPosLocation,1,viewPos);
		glUniform4fv(lightPosLocation,1,lightPos);
		glUniform4fv(lightColorLocation,1,lightColor);
		glUniform4fv(lightDirectionLocation,1,lightDirection);
		glUniform1f(spotLightCutoffLocation, spotLightCutoff);
		glUniform1f(lightIntensityLocation, lightIntensity);

		glUniform4fv(diffuseColorLocation,1,diffuseColor);
		glUniform4fv(diffuseMaterialLocation,1,materialColor);

		glUniform4fv(specularColorLocation,1,specularColor);
		glUniform4fv(specularMaterialLocation,1,specularMaterialColor);

		glUniform4fv(fogColorLocation,1,fogColor);
		glUniform4fv(fogDistanceLocation,1,fogDistance);
		glUniform1f(fogDensityLocation, fogDensity);
		glUniform1f(fogGradientLocation,fogGradient);

		// bind ibo
		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);

		/*glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,texture->mTextureID);
		glUniform1i(MainTextureLocation,0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, secondTex->mTextureID);
		glUniform1i(SecondTextureLocation,1);*/
 
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, (void*)0);
		glBindVertexArray(0);

		glUniformMatrix4fv(MLocation, 1, GL_FALSE, glm::value_ptr(MODEL1));
		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, (void*)0);
		glBindVertexArray(0);

		glUniformMatrix4fv(MLocation, 1, GL_FALSE, glm::value_ptr(MODEL2));
		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, (void*)0);
		glBindVertexArray(0);

		glUseProgram(0);
		glFinish();
	};
 
	auto renderTopLeft = [&](void) 
	{
		glUseProgram(TLProgram.mProgram);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->mTextureID);
		glUniform1i(TLProgram.GetLocation(MAIN_TEXTURE), 0);

	/*	glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, secondTex->mTextureID);
		glUniform1i(TLProgram.GetLocation(SECOND_TEXTURE),1);*/

		fullscreen.Draw(TLProgram.GetLocation(VERTEX), TLProgram.GetLocation(TEXCOORD),new Rect(-1.0f,0.0f,1.0f,0.0f));
		glUseProgram(0);
	}; 

	auto renderTopRight = [&](void) 
	{
		glUseProgram(TRProgram.mProgram);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->mTextureID);
		glUniform1i(TRProgram.GetLocation(MAIN_TEXTURE), 0);

		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, secondTex->mTextureID);
		//glUniform1i(TRProgram.GetLocation(SECOND_TEXTURE), 1);

		fullscreen.Draw(TRProgram.GetLocation(VERTEX), TRProgram.GetLocation(TEXCOORD), new Rect(0.0f, 1.0f, 1.0f, 0.0f));
		glUseProgram(0);
	};

	auto renderBottomLeft = [&](void) 
	{
		glUseProgram(BLProgram.mProgram);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->mTextureID);
		glUniform1i(BLProgram.GetLocation(MAIN_TEXTURE), 0);

		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, secondTex->mTextureID);
		//glUniform1i(BLProgram.GetLocation(SECOND_TEXTURE), 1);

		fullscreen.Draw(BLProgram.GetLocation(VERTEX), BLProgram.GetLocation(TEXCOORD), new Rect(-1.0f, 0.0f, 0.0f, -1.0f));
		glUseProgram(0);
	};

	auto renderBottomRight = [&](void) 
	{ 
		glUseProgram(BRProgram.mProgram);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->mTextureID);
		glUniform1i(BRProgram.GetLocation(MAIN_TEXTURE), 0);

	/*	glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, secondTex->mTextureID);
		glUniform1i(BRProgram.GetLocation(SECOND_TEXTURE), 1);*/

		fullscreen.Draw(BRProgram.GetLocation(VERTEX), BRProgram.GetLocation(TEXCOORD), new Rect(0.0f, 1.0f, 0.0f, -1.0f));
		glUseProgram(0);
	};

	auto renderSkybox = [&](void) 
	{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f,0.0f,0.0f,0.0f);
		glDisable(GL_DEPTH_TEST);

		glUseProgram(skyboxProgram.mProgram);

		glUniformMatrix4fv(skyboxProgram.GetLocation("M"),1,GL_FALSE,glm::value_ptr(glm::vec4(1.0f)));
		glUniformMatrix4fv(skyboxProgram.GetLocation("V"),1,GL_FALSE,glm::value_ptr(glm::vec4(1.0f)));
		glUniformMatrix4fv(skyboxProgram.GetLocation("P"),1,GL_FALSE,glm::value_ptr(glm::vec4(1.0f)));

		glUniform3fv(skyboxProgram.GetLocation("U_EyePos"),1, viewPos);

		glBindVertexArray(vao);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubemap);
		glUniform1i(skyboxProgram.GetLocation("MAIN_TEXTURE"), 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, (void*)0);
		glBindVertexArray(0);

		glUseProgram(0);
	};

	glEnable(GL_CULL_FACE); 
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

		renderSkybox();

		glDisable(GL_BLEND);
		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
		//glClearColor(41.0f / 255.0f, 71.0f / 255.0f, 121.0f / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

		render();

		//// top left
		//renderTopLeft();

		//// top right
		//renderTopRight();

		//// bottom left
		//renderBottomLeft();

		//// bottom right
		//renderBottomRight();

		glFinish();
		SwapBuffers(dc);
	}
	return  0;
}