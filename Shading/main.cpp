
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

	// create GPU program
	GLuint program = CreateGPUProgram("res/shader/vertex/vertexcolor.vs", 
									  "res/shader/vertex/vertexcolor.fs");
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

	//GPUProgram cube;
	//cube.AttachShader(GL_VERTEX_SHADER, "res/shader/vertex/vertexcolor.vs");
	//cube.AttachShader(GL_FRAGMENT_SHADER, "res/shader/vertex/vertexcolor.fs");
	//cube.LinkProgram();
	//cube.InitializeLocation();
	
	GLuint cube = CreateGPUProgram(
		/*"res/shader/skybox/cubemap_reflection.vs",
		"res/shader/skybox/cubemap_reflection.fs");*/
		"res/shader/skybox/refraction.vs",
		"res/shader/skybox/refraction.fs");
	GLuint cubeMLocation, cubeVLocation, cubePLocation, cubeNMLocation, cubevertexLocation, cubenormalLocation, cubetexcoordLocation, cubeMainTextureLocation, cubeSecondTextureLocation;
	cubevertexLocation = glGetAttribLocation(cube, "vertex");
	cubenormalLocation = glGetAttribLocation(cube, "normal");
	cubetexcoordLocation = glGetAttribLocation(cube, "texcoord");
	cubeMLocation = glGetUniformLocation(cube, "M");
	cubeVLocation = glGetUniformLocation(cube, "V");
	cubePLocation = glGetUniformLocation(cube, "P");
	cubeNMLocation = glGetUniformLocation(cube, "NM");
	cubeMainTextureLocation = glGetUniformLocation(cube, "U_MainTexture");
	cubeSecondTextureLocation = glGetUniformLocation(cube, "U_SecondTexture");
	GLuint cubeCameraLocation = glGetUniformLocation(cube, "U_CameraPos");

	// load cube model
	unsigned int* indices = nullptr;
	int indexCount, vertexCount;
	VertexData* vertices = LoadObjModel("res/model/quad.obj", &indices, indexCount, vertexCount);
	 
	// load sphere model
	unsigned int* cubeIndices = nullptr;
	int cubeVertexCount, cubeIndexCount;
	VertexData* cubeVertices = LoadObjModel("res/model/sphere.obj", &cubeIndices, cubeIndexCount, cubeVertexCount);
	
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

	GLuint VAO = CreateVAO([&](void)
	{
		GLuint VBO = CreateBufferObject(GL_ARRAY_BUFFER, sizeof(VertexData) * cubeVertexCount, GL_STATIC_DRAW, cubeVertices);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);  

		glEnableVertexAttribArray(cubevertexLocation);
		glVertexAttribPointer(cubevertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);

		glEnableVertexAttribArray(cubenormalLocation);
		glVertexAttribPointer(cubenormalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(float) * 3));

		glEnableVertexAttribArray(cubetexcoordLocation);
		glVertexAttribPointer(cubetexcoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(float) * 6));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	});
	GLuint IBO = CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * cubeIndexCount, GL_STATIC_DRAW, cubeIndices);
	 

	glClearColor(41.0f / 255.0f, 71.0f / 255.0f, 121.0f / 255.0f, 1.0f);
	glViewport(0, 0, width, height);
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd); 

	glm::mat4 MODEL0 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) *glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 MODEL1 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f))*glm::rotate(glm::mat4(1.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 MODEL2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -15.0f)) * glm::rotate(glm::mat4(1.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 PROJECTION = glm::perspective(45.0f, float(width) / (float)height, 0.1f, 200.0f);
	glm::mat4 ORTHO = glm::ortho(-width/2.0f, width/2.0f, -height/2.0f, height/2.0f);
	glm::mat4 NM = glm::inverseTranspose(MODEL0);

	//glm::mat4 VIEW = glm::mat4(1.0);
	glm::mat4 VIEW = glm::lookAt(glm::vec3(0.0f, 0.0f,0.0f), glm::vec3(0.0f,0.0f,-1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	float viewPos[] = {0.0f,0.0f,.0f};
	// LIGHT SETTING
	float lightPos[] = {2.0f,2.0f,-3.0f,0.0f};
	float lightColor[] = {1.0f,1.0f ,1.0f,1.0f};
	float lightDirection[] = {0.0f,-1.0f,0.0f,128.0f};
	float lightIntensity = 3.0f;

	// SPOT LIGTH SETTING	
	float spotLightCutoff = 15.0f;

	// AMBIENT SETTING
	float ambientColor[] = { 0.12f,0.12f,0.12f,1.0f };

	// DIFFUSE SETTING
	float materialColor[] = {0.4f,0.4f,0.4f,1.0f};
	float diffuseColor[] = {0.1f,0.4f,0.7f,1.0f};

	// SPECULAR SETTING
	float specularColor[] = { 0.75f,0.75f,0.75f,1.0f };
	float specularMaterialColor[] = {1.0f,1.0f,1.0f,1.0f};

	// FOG SETTING
	float fogColor[] = {0.6f,0.6f,0.6f,1.0f};
	float fogDistance[] = {0.0f,30.0f,0.0f,0.0f};
	float fogDensity = 0.1f;
	float fogGradient = 2.0f; 

	//auto renderSkybox = [&](void) 
	//{ 
	//	glDisable(GL_DEPTH_TEST);

	//	glUseProgram(skyboxProgram.mProgram);

	//	glUniformMatrix4fv(skyboxProgram.GetLocation("M"),1,GL_FALSE,glm::value_ptr(MODEL0));
	//	glUniformMatrix4fv(skyboxProgram.GetLocation("V"),1,GL_FALSE,glm::value_ptr(VIEW));
	//	glUniformMatrix4fv(skyboxProgram.GetLocation("P"),1,GL_FALSE,glm::value_ptr(PROJECTION));

	//	glUniform3fv(skyboxProgram.GetLocation("U_EyePos"),1, viewPos);

	//	glBindVertexArray(vao);

	//	//glActiveTexture(GL_TEXTURE0);
	//	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
	//	glUniform1i(skyboxProgram.GetLocation(MAIN_TEXTURE), 0);

	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	//	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, (void*)0);
	//	glBindVertexArray(0);

	//	glUseProgram(0);
	//}; 
	 
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

	glm::mat4 receiverModel = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,-0.3f,-3.0f)) * 
		glm::rotate(glm::mat4(1.0f) , 30.0f,glm::vec3(1.0f,0.0f,0.0f)) * 
		glm::scale(glm::mat4(1.0f) , glm::vec3(2.5f));
	glm::mat4 receiverView = glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 receiverProjection = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 200.0f);
	glm::mat4 receiverNM = glm::inverseTranspose(receiverModel);

	auto renderShadowReceiver = [&](void) 
	{
		glUseProgram(receiver.mProgram);

		glUniformMatrix4fv(receiver.GetLocation(M_MATRIX),1,GL_FALSE,glm::value_ptr(receiverModel));
		glUniformMatrix4fv(receiver.GetLocation(V_MATRIX), 1, GL_FALSE, glm::value_ptr(receiverView));
		glUniformMatrix4fv(receiver.GetLocation(P_MATRIX), 1, GL_FALSE, glm::value_ptr(receiverProjection));
		//glUniformMatrix4fv(receiver.GetLocation(NM_MATRIX), 1, GL_FALSE, glm::value_ptr(receiverNM));

		glUniform4fv(receiver.GetLocation("U_AmbientColor"), 1, ambientColor);
		glUniform4fv(receiver.GetLocation("U_LightPos"), 1, lightPos);
		glUniform4fv(receiver.GetLocation("U_LightColor"),1,lightColor);
		glUniform4fv(receiver.GetLocation("U_DiffuseColor"),1,diffuseColor);
		glUniform4fv(receiver.GetLocation("U_SpecularColor"), 1, specularColor);

		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
		glDrawElements(GL_TRIANGLES, indexCount,GL_UNSIGNED_INT,(void*)0);
		glBindVertexArray(0);
		glUseProgram(0);
	};

	GPUProgram caster;
	caster.AttachShader(GL_VERTEX_SHADER, "res/shader/diffuse.vs");
	caster.AttachShader(GL_FRAGMENT_SHADER, "res/shader/diffuse.fs");
	caster.LinkProgram();
	caster.InitializeLocation(); 

	glm::mat4 casterModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, -3.0f)) *
		glm::rotate(glm::mat4(1.0f), 30.0f, glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(0.3f));
	glm::mat4 casterView = glm::lookAt(glm::vec3(0.0f),glm::vec3(0.0f,0.0f,-3.0f),glm::vec3(0.0f,1.0f,0.0f));
	glm::mat4 casterProjection = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 200.0f);
	glm::mat4 casterNM = glm::inverseTranspose(casterModel);

	auto renderShadowCast = [&](void) 
	{
		glUseProgram(caster.mProgram);

		glUniformMatrix4fv(caster.GetLocation(M_MATRIX), 1, GL_FALSE, glm::value_ptr(casterModel));
		glUniformMatrix4fv(caster.GetLocation(V_MATRIX), 1, GL_FALSE, glm::value_ptr(casterView));
		glUniformMatrix4fv(caster.GetLocation(P_MATRIX), 1, GL_FALSE, glm::value_ptr(casterProjection));
		//glUniformMatrix4fv(caster.GetLocation(NM_MATRIX), 1, GL_FALSE, glm::value_ptr(casterNM)); 
		
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);

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

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f); 

		glEnable(GL_TEXTURE_2D);
		// TODO: draw quad as shadow_receive
		renderShadowReceiver();
		// TODO: draw sphere as shadow_cast
		renderShadowCast();
		// TODO: draw origin screen in left top window.
		// TODO: draw depth buffer in right top window.

#pragma region ∑÷∆¡‰÷»æ

		//// top left
		//renderTopLeft();

		//// top right
		//renderTopRight();

		//// bottom left
		//renderBottomLeft();

		//// bottom right
		//renderBottomRight();

#pragma endregion

		glFinish();
		SwapBuffers(dc);
	}
	return  0;
}