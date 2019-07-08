#include "gpuprogram.h"
#include "util.h"
#include "engine.h"

GPUProgram::GPUProgram() 
{
	mProgram = glCreateProgram();
}

GLuint GPUProgram::CompileShader(GLenum shaderType,const char*shaderFile) 
{
	const char* shaderCode = LoadFileContent(shaderFile);
	if( shaderCode == nullptr )
	{
		Debug::LogError("Comile shader error");
		return 0;
	}
	GLuint shader = glCreateShader(shaderType);
	if (shader == 0) 
	{
		Debug::LogError("Create shader error");
		return 0;
	}
	glShaderSource(shader, 1, &shaderCode, nullptr);
	glCompileShader(shader);
	GLint compileResult = GL_TRUE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
	if (compileResult == GL_FALSE) 
	{
		char szLog[1024]{0};
		GLsizei logLen = 0;
		glGetShaderInfoLog(shader, 1024, &logLen, szLog);
		Debug::LogError(szLog);
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}

void GPUProgram::AttachShader(GLenum shaderType,const char*shaderFile) 
{
	GLuint shader = this->CompileShader(shaderType, shaderFile);
	glAttachShader(this->mProgram, shader);
	glDetachShader(this->mProgram, shader);
	glDeleteShader(shader);
}

void GPUProgram::LinkProgram() 
{
	glLinkProgram(this->mProgram);
	GLint linkResult = GL_TRUE;
	glGetProgramiv(this->mProgram, GL_LINK_STATUS, &linkResult);
	if (linkResult == GL_FALSE)
	{
		char szLog[1024]{0};
		GLsizei logLen = 0;
		glGetProgramInfoLog(this->mProgram, 1024, &logLen, szLog);
		Debug::LogError("Link GPU Program Error");
		glDeleteProgram(this->mProgram);
	}
}

void GPUProgram::DetectAttribute( const char*attName ) 
{

}

void GPUProgram::DetectUniform( const char*uniformName ) 
{

}