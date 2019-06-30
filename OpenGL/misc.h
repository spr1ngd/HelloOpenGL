#pragma once
#include "glew.h"
#include <functional>

GLuint CreateBufferObject(GLenum target,GLsizeiptr size,GLenum usage, const void*data = nullptr);

GLuint CreateVAO(std::function<void()>setting);

GLuint CreateFrameBufferObject(GLuint&colorBuffer,GLuint&depthBuffer,int width,int height);

GLuint CompileShader(GLenum shaderType,const char* shaderPath);

GLuint CreateGPUProgram(const char* vsFile, const char* fsFile);
GLuint CreateComputerShaderProgram(const char* computerShaderFile);

void CheckGLError(const char*file,int line);
#define GL_CALL(x) do{x;CheckGLError(__FILE__,__LINE__);}while (0)