#pragma once
#include "glew.h"
#include "rect.h"

class FullScreenQuad
{
public: 
	GLuint mVBO;
public:
	void Init();
	void Draw(GLuint posLocation,GLuint texcoordLocation);
	void Draw(GLuint posLocation,GLuint texcoordLocation,Rect* rect);
};

