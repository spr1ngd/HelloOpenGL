#pragma once
#include "glew.h"
#include "rect.h"

class FullScreenQuad
{
public: 
	GLuint mVBO;
public:
	void Init();
	void Draw(GLuint posLocation);
	//void Draw(GLuint posLocation,Rect* rect);
};

