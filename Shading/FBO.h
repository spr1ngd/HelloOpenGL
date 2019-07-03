#pragma once
#include "glew.h"
#include <map>
#include <stack>
#include <string>

class FBO
{
public:
	std::map<std::string, GLuint> mBuffer;
	std::stack<GLenum> mDrawBuffers;
public:
	void AttachColorBuffer();
	void AttachDepthBuffer();
};

