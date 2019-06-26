#pragma once
//#include <windows.h>
//#include <gl/GL.h>
#include "glew.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"

class Frustum
{
public:
	GLuint mVBO, mIBO ,mProgram;
	GLint mVertexLocation,mMLocation, mVLocation, mPLocation;
public:
	Frustum();
	void InitProgram();
	void InitPerspective(float fov,float aspect,float zNear,float zFar);
	void Draw(float*model,float*view,float*projection);
};

