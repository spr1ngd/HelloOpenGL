#pragma once
#include "glew.h"
#include <map>
#include <string>
#include <vector>

class FBO
{
public:
	FBO();
	GLuint mFBO;
	std::map<std::string, GLuint> mBuffer;
	std::vector<GLenum> mDrawBuffers;
public:
	void AttachColorBuffer(const char*bufferName,GLenum attachment,GLenum dataType,int width,int height);
	void AttachDepthBuffer(const char*bufferName,int widht,int height);
	void Finish();
	void Bind();
	void Unbind();
	GLuint GetBuffer(const char* bufferName);
};

