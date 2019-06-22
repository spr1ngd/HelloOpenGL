#pragma once
#include "glew.h"

GLuint CreateBufferObject(GLenum target,GLsizeiptr size,GLenum usage, const void*data = nullptr);

GLuint CompileShader(GLenum shaderType,const char* shaderPath);

GLuint CreateGPUProgram(const char* vsFile, const char* fsFile);