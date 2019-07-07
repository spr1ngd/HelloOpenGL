#include "gpuprogram.h"

GPUProgram::GPUProgram() 
{
	mProgram = glCreateProgram();
}

GLuint GPUProgram::CompileShader(GLenum shaderType,const char*shaderFile) 
{

}

void GPUProgram::AttachShader(GLenum shaderType,const char*shaderFile) 
{
	GLuint shaderProgram = this->CompileShader(shaderType, shaderFile);
	if (mProgram == 0)
	{
		// TODO: ¸ÄÎªDebug.Log();
		printf("");
		return;
	}
}

void GPUProgram::DetectAttribute( const char*attName ) 
{

}

void GPUProgram::DetectUniform( const char*uniformName ) 
{

}