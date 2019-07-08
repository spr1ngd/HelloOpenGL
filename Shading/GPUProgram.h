#pragma once
#include "glew.h"
#include "stdio.h"

class GPUProgram 
{
private:
	GLuint mMLocation, mVLocation, mPLocation,
		mVertexLocation, mNormalLocation, mTexcoordLocation,
		mMainTextureLocation, mSecondTextureLocation;
	GLuint CompileShader(GLenum shaderType,const char*shaderFile);
public:
	GPUProgram();
	GLuint mProgram;
public:
	void AttachShader(GLenum shaderType,const char*shaderFile);
	void LinkProgram();
	void DetectAttribute(const char*attName);
	void DetectUniform(const char*uniformName);
};