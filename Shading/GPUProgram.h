#pragma once
#include "glew.h"
#include "stdio.h"
#include <map>

#define VERTEX "vertex"
#define NORMAL "normal"
#define TEXCOORD "texcoord"
#define M_MATRIX "M"
#define V_MATRIX "V"
#define P_MATRIX "P"
#define NM_MATRIX "NM"
#define MAIN_TEXTURE "U_MainTexture"
#define SECOND_TEXTURE "U_SecondTexture"

class GPUProgram 
{
private:
	std::map<const char*, GLuint> mLocations;
	GLuint CompileShader(GLenum shaderType,const char*shaderFile);
	void InitializeLocation();
public:
	GPUProgram();
	GLuint mProgram;
public:
	void AttachShader(GLenum shaderType,const char*shaderFile);
	void LinkProgram();
	void DetectAttribute(const char*attName);
	void DetectUniform(const char*uniformName);
	void EnableVertexAttrib(const char*attName,GLint size,GLsizei stride,const void* );
	void EnableUniform(const char*uniformName,const GLfloat*);
	GLuint GetLocation(const char*name);
};