
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
	GLuint program = CreateGPUProgram("res/shader/light.vs", 
									  "res/shader/light/hdr_light.fs");
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

	// load cube model
	unsigned int* indices = nullptr;
	int indexCount, vertexCount;
	VertexData* vertices = LoadObjModel("res/model/Sphere.obj", &indices, indexCount, vertexCount);
	Texture* texture = Texture::LoadTexture("res/texture/fur.jpg");
	Texture* secondTex = Texture::LoadTexture("res/texture/carbon_fiber.jpg");

	GPUProgram fsProgram; 
	fsProgram.AttachShader(GL_VERTEX_SHADER, "res/shader/fullscreen.vs");
	fsProgram.AttachShader(GL_FRAGMENT_SHADER,"res/shader/fullscreen.fs");
	fsProgram.LinkProgram();
	fsProgram.InitializeLocation();

	GPUProgram erosionProgram;
	erosionProgram.AttachShader(GL_VERTEX_SHADER,"res/shader/fullscreen.vs");
	erosionProgram.AttachShader(GL_FRAGMENT_SHADER,"res/shader/erosion.fs");
	erosionProgram.LinkProgram();
	erosionProgram.InitializeLocation();

	GPUProgram dilationProgram;
	dilationProgram.AttachShader(GL_VERTEX_SHADER,"res/shader/fullscreen.vs");
	dilationProgram.AttachShader(GL_FRAGMENT_SHADER,"res/shader/dilation.fs");
	dilationProgram.LinkProgram();
	dilationProgram.InitializeLocation();

	GPUProgram gaussianProgram;
	gaussianProgram.AttachShader(GL_VERTEX_SHADER,"res/shader/fullscreen.vs");
	gaussianProgram.AttachShader(GL_FRAGMENT_SHADER,"res/shader/blur/gaussian.fs");
	gaussianProgram.LinkProgram();
	gaussianProgram.InitializeLocation();

	GPUProgram easyGaussianProgram;
	easyGaussianProgram.AttachShader(GL_VERTEX_SHADER,"res/shader/fullscreen.vs");
	easyGaussianProgram.AttachShader(GL_FRAGMENT_SHADER,"res/shader/blur/easy_gaussian.fs");
	easyGaussianProgram.LinkProgram();
	easyGaussianProgram.InitializeLocation();

	GPUProgram hdrProgram;
	hdrProgram.AttachShader(GL_VERTEX_SHADER,"res/shader/fullscreen.vs");
	hdrProgram.AttachShader(GL_FRAGMENT_SHADER,"res/shader/hdr/hdr_composite.fs");
	hdrProgram.LinkProgram();
	hdrProgram.InitializeLocation();
	hdrProgram.DetectUniform("U_HDRTetxure");

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

	glm::mat4 MODEL = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f)) * glm::rotate(glm::mat4(1.0f), 30.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		//*glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f));
	glm::mat4 PROJECTION = glm::perspective(45.0f, float(width) / (float)height, 0.1f, 200.0f);
	glm::mat4 ORTHO = glm::ortho(-width/2.0f, width/2.0f, -height/2.0f, height/2.0f);
	glm::mat4 VIEW = glm::mat4(1.0f);
	glm::mat4 NM = glm::inverseTranspose(MODEL);

	float viewPos[] = {2.0f,2.0f,-2.0f};

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
	float diffuseColor[] = {0.5f,0.5f,0.5f,1.0f};

	// SPECULAR SETTING
	float specularColor[] = { 0.75f,0.75f,0.75f,1.0f };
	float specularMaterialColor[] = {1.0f,1.0f,1.0f,1.0f};

	auto render = [&](void)
	{ 
		glUseProgram(program);
		glUniformMatrix4fv(MLocation, 1, GL_FALSE, glm::value_ptr(MODEL));
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

		glUseProgram(0);
		glFinish();
	};
 
	auto renderTopLeft = [&](void) 
	{
		glUseProgram(fsProgram.mProgram);
		//glBindBuffer(GL_ARRAY_BUFFER,fullscreen.mVBO);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_DEPTH_TEST);
		glActiveTexture(GL_TEXTURE0);
		GLuint cb = hdrFBO.GetBuffer("color");
		glBindTexture(GL_TEXTURE_2D, cb); 
		fullscreen.Draw(fsProgram.GetLocation(VERTEX), fsProgram.GetLocation(TEXCOORD),new Rect(-1.0f,0.0f,1.0f,0.0f));
		//glBindBuffer(GL_ARRAY_BUFFER,0);
		glUseProgram(0);
	}; 

	auto renderTopRight = [&](void) 
	{
		glUseProgram(fsProgram.mProgram);
		glActiveTexture(GL_TEXTURE0);
		GLuint cb = hdrFBO.GetBuffer("hdrColor");
		glBindTexture(GL_TEXTURE_2D, cb);
		glUniform1i(fsProgram.GetLocation(MAIN_TEXTURE), 0);
		fullscreen.Draw(fsProgram.GetLocation(VERTEX), fsProgram.GetLocation(TEXCOORD),new Rect(0.0f,1.0f,1.0f,0.0f));
		glUseProgram(0);
	};

	auto renderBottomLeft = [&](void) 
	{
		fbo2.Bind();
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(easyGaussianProgram.mProgram);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, hdrFBO.GetBuffer("hdrColor"));
		glUniform1i(easyGaussianProgram.GetLocation(MAIN_TEXTURE), 0);
		fullscreen.Draw(easyGaussianProgram.GetLocation(VERTEX), easyGaussianProgram.GetLocation(TEXCOORD));
		glUseProgram(0);
		fbo2.Unbind();

		glUseProgram(hdrProgram.mProgram);
		glActiveTexture(GL_TEXTURE0);
		GLuint cb = hdrFBO.GetBuffer("color");
		glBindTexture(GL_TEXTURE_2D, cb);
		glUniform1i(hdrProgram.GetLocation(MAIN_TEXTURE), 0);

		glActiveTexture(GL_TEXTURE1); 
		GLuint hdrbuffer = fbo2.GetBuffer("color");
		// GLuint hdrbuffer = hdrFBO.GetBuffer("hdrColor");
		glBindTexture(GL_TEXTURE_2D,hdrbuffer);
		glUniform1i(hdrProgram.GetLocation("U_HDRTexture"), 1);

		fullscreen.Draw(hdrProgram.GetLocation(VERTEX), hdrProgram.GetLocation(TEXCOORD), new Rect(-1.0f,0.0f,0.0f,-1.0f));
		glUseProgram(0);
	};

	auto renderBottomRight = [&](void) 
	{ 
		fbo1.Bind();
		glUseProgram(fsProgram.mProgram);
		glActiveTexture(GL_TEXTURE0);
		GLuint cb = hdrFBO.GetBuffer("hdrColor");
		glBindTexture(GL_TEXTURE_2D, cb);
		glUniform1i(fsProgram.GetLocation(MAIN_TEXTURE), 0);
		fullscreen.Draw(fsProgram.GetLocation(VERTEX), fsProgram.GetLocation(TEXCOORD));
		glUseProgram(0);
		fbo1.Unbind();

		glUseProgram(easyGaussianProgram.mProgram);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,fbo1.GetBuffer("color"));
		glUniform1i(easyGaussianProgram.GetLocation(MAIN_TEXTURE),0);
		fullscreen.Draw(fsProgram.GetLocation(VERTEX),fsProgram.GetLocation(TEXCOORD), new Rect(0.0f, 1.0f, 0.0f, -1.0f));
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

		//fbo.Bind();
		hdrFBO.Bind();
		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearColor(41.0f / 255.0f, 71.0f / 255.0f, 121.0f / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		/*glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);*/
		render();
		hdrFBO.Unbind();
		//fbo.Unbind();

	/*	fbo1.Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(gaussianProgram.mProgram);
		glBindTexture(GL_TEXTURE_2D, fbo.GetBuffer("color"));
		glUniform1i(fsProgram.GetLocation(MAIN_TEXTURE), 0);
		fullscreen.Draw(gaussianProgram.GetLocation(VERTEX), gaussianProgram.GetLocation(TEXCOORD));
		glUseProgram(0);
		fbo1.Unbind();

		fbo2.Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(gaussianProgram.mProgram);
		glBindTexture(GL_TEXTURE_2D, fbo1.GetBuffer("color"));
		glUniform1i(fsProgram.GetLocation(MAIN_TEXTURE), 0);
		fullscreen.Draw(gaussianProgram.GetLocation(VERTEX), gaussianProgram.GetLocation(TEXCOORD));
		glUseProgram(0);
		fbo2.Unbind();

		fbo3.Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(gaussianProgram.mProgram);
		glBindTexture(GL_TEXTURE_2D, fbo2.GetBuffer("color"));
		glUniform1i(fsProgram.GetLocation(MAIN_TEXTURE), 0);
		fullscreen.Draw(gaussianProgram.GetLocation(VERTEX), gaussianProgram.GetLocation(TEXCOORD));
		glUseProgram(0);
		fbo3.Unbind();*/

		glDisable(GL_BLEND);
		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//glClearColor(41.0f / 255.0f, 71.0f / 255.0f, 121.0f / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		// top left
		// TODO : orginal 
		renderTopLeft();

		// top right
		// TODO : hdr region 
		renderTopRight();

		// bottom left
		// TODO : composite picture
		renderBottomLeft();

		// bottom right
		renderBottomRight();

		glFinish();
		SwapBuffers(dc);
	}
	return  0;
}