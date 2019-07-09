#pragma once
#include "glew.h"

class FullScreenQuad
{
public: 
	GLuint mVBO;
public:
	void Init();
	void Draw(GLuint posLocation,GLuint texcoordLocation);
};

