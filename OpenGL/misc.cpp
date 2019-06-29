#include "misc.h"
#include "util.h"
#include <stdio.h>

GLuint CreateBufferObject(GLenum target, GLsizeiptr size, GLenum usage,const void* data)
{
	GLuint object;
	glGenBuffers(1, &object);
	glBindBuffer(target, object);
	glBufferData(target, size, data, usage);
	glBindBuffer(target, 0);
	return object;
}

GLuint CreateVAO( std::function<void()> setting) 
{
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	setting();
	glBindVertexArray(0);
	return vao;
}

GLuint CreateFrameBufferObject( GLuint&colorBuffer,GLuint&depthBuffer,int width,int height )
{
	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	// color buffer
	glGenTextures(1, &colorBuffer);
	
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D,0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
	// depth buffer
	glGenTextures(1,&depthBuffer);
	glBindTexture(GL_TEXTURE_2D, depthBuffer); 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH24_STENCIL8,width,height,0, GL_DEPTH_STENCIL,GL_UNSIGNED_INT_24_8,NULL);
	glBindTexture(GL_TEXTURE_2D,0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("create framebuffer object fail.\n");
	}
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	return fbo;
}

GLuint CompileShader(GLenum shaderType, const char* shaderPath) 
{
	const char* shaderCode = LoadFileContent(shaderPath);
	if (shaderCode == nullptr) 
	{
		printf("[misc.cpp]: loadFileContent file:[%s] fail.",shaderPath);
		return 0;
	}
	GLuint shader = glCreateShader(shaderType);
	if (shader == 0)
	{
		return 0;
	}
	glShaderSource(shader, 1, &shaderCode, nullptr);
	glCompileShader(shader);
	GLint compileResult = GL_TRUE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
	if (compileResult == GL_FALSE)
	{
		char szLog[1024] = { 0 };
		GLsizei logLen = 0;
		glGetShaderInfoLog(shader, 1024,&logLen,szLog);
		printf("[misc.cpp]: glCompileShader shader file:[%s] fail.\n fail content :\n%s", shaderPath, szLog);
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}

GLuint LinkGPUProgram( GLuint vsShader,GLuint fsShader ) 
{
	GLuint program = glCreateProgram();
	glAttachShader(program, vsShader);
	glAttachShader(program, fsShader);
	glLinkProgram(program);
	GLint linkResult = GL_TRUE;
	glGetProgramiv(program, GL_LINK_STATUS, &linkResult);
	if (linkResult == GL_FALSE) 
	{
		char szLog[1024] = { 0 };
		GLsizei logLen = 0;
		glGetProgramInfoLog(program, 1024, &logLen, szLog);
		printf("[misc.cpp]: link gpu program fail. fail error : %s\n",szLog);
		glDeleteProgram(program);
		return 0;
	}
	return program;
}


GLuint CreateGPUProgram(const char* vsFile, const char* fsFile)
{
	GLuint vsShader = CompileShader(GL_VERTEX_SHADER,vsFile);
	GLuint fsShader = CompileShader(GL_FRAGMENT_SHADER,fsFile); 
	GLuint program = LinkGPUProgram(vsShader, fsShader);
	if (program == 0)
		return 0;
	glDetachShader(program, vsShader);
	glDetachShader(program, fsShader);
	glDeleteShader(vsShader);
	glDeleteShader(fsShader);
	return program;
}

void CheckGLError(const char* file, int line)
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		switch (error)
		{
		case GL_INVALID_ENUM:
			printf("GL error GL_INVALID_ENUM %s line%d\n",file,line);
			break;  
		case GL_INVALID_VALUE:
			printf("GL error GL_INVALID_VALUE %s line%d\n", file, line);
			break;
		case GL_INVALID_OPERATION:
			printf("GL error GL_INVALID_OPERATION %s line%d\n", file, line);
			break;
		case GL_STACK_OVERFLOW:
			printf("GL error GL_STACK_OVERFLOW %s line%d\n", file, line);
			break;
		case GL_STACK_UNDERFLOW:
			printf("GL error GL_STACK_UNDERFLOW %s line%d\n", file, line);
			break;
		case GL_OUT_OF_MEMORY:
			printf("GL error GL_OUT_OF_MEMORY %s line%d\n", file, line);
			break;
		default:
			printf("GL error GL_ERROR 0x%x\n",error);
			break;
		}
	}
}