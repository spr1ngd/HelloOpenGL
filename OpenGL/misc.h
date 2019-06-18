#pragma once
#include "glew.h"

GLuint CreateBufferObject(GLenum target,GLsizeiptr size,GLenum usage, const void*data = nullptr);

char* LoadFileContent(const char* filePath);