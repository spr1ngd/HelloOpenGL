#include "gpuprogram.h"
#include "util.h"
#include "engine.h"

GPUProgram::GPUProgram() 
{
	mProgram = glCreateProgram();
	this->InitializeLocation();
}

void GPUProgram::InitializeLocation() 
{
	// vertex
	GLuint vertexLocation = glGetAttribLocation(this->mProgram, VERTEX);
	this->mLocations.insert(std::pair<const char*,GLuint>(VERTEX, vertexLocation));

	// normal
	GLuint normalLocation = glGetAttribLocation(this->mProgram,NORMAL);
	this->mLocations.insert(std::pair<const char*,GLuint>(NORMAL,normalLocation));

	// texcoord
	GLuint texcoordLocation = glGetAttribLocation(this->mProgram,TEXCOORD);
	this->mLocations.insert(std::pair<const char*,GLuint>(TEXCOORD,texcoordLocation));

	// M
	GLuint mMatrixLocation = glGetUniformLocation(this->mProgram,M_MATRIX);
	this->mLocations.insert(std::pair<const char*,GLuint>(M_MATRIX,mMatrixLocation));

	// V
	GLuint vMatrixLocation = glGetUniformLocation(this->mProgram,V_MATRIX);
	this->mLocations.insert(std::pair<const char*,GLuint>(V_MATRIX,vMatrixLocation));

	// P
	GLuint pMatrixLocation = glGetUniformLocation(this->mProgram,P_MATRIX);
	this->mLocations.insert(std::pair<const char*,GLuint>(P_MATRIX,pMatrixLocation));

	// U_MainTexture
	GLuint mainTexLocation = glGetUniformLocation(this->mProgram,MAIN_TEXTURE);
	this->mLocations.insert(std::pair<const char*,GLuint>(MAIN_TEXTURE,mainTexLocation));

	// U_SecondTexture
	GLuint secondTexLocation = glGetUniformLocation(this->mProgram,SECOND_TEXTURE);
	this->mLocations.insert(std::pair<const char*,GLuint>(SECOND_TEXTURE,secondTexLocation));
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
	this->mShader[shaderType] = shader;
}

void GPUProgram::LinkProgram() 
{
	glLinkProgram(this->mProgram);
	GLint linkResult = GL_TRUE;
	glGetProgramiv(this->mProgram, GL_LINK_STATUS, &linkResult);
	if (linkResult == GL_FALSE)
	{
		char szLog[1024]={0};
		GLsizei logLen = 0;
		glGetProgramInfoLog(this->mProgram, 1024, &logLen, szLog);
		printf("LINK ERROR : %s\n", szLog);
		Debug::LogError("Link GPU Program Error");
		glDeleteProgram(this->mProgram);
	}
	for (auto& v : this->mShader)
	{
		glDetachShader(this->mProgram, v.second);
		glDeleteShader(v.second);
	}
	this->mShader.clear();
}

void GPUProgram::DetectAttribute( const char*attName ) 
{
	GLuint location = glGetAttribLocation(this->mProgram,attName);
	this->mLocations.insert(std::pair<const char*,GLuint>(attName,location));
}

void GPUProgram::DetectUniform( const char*uniformName ) 
{
	GLuint location = glGetUniformLocation(this->mProgram,uniformName);
	this->mLocations.insert(std::pair<const char*,GLuint>(uniformName,location));
}

void GPUProgram::EnableVertexAttrib(const char* attName, GLint size, GLsizei stride, const void* pinter) 
{
	GLuint location = this->mLocations[attName];
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, stride, pinter);
}

void GPUProgram::EnableUniform(const char* uniformName, const GLfloat*value)
{
	GLuint location = this->mLocations[uniformName];
	glUniformMatrix4fv(location,1,GL_FALSE,value);
}

GLuint GPUProgram::GetLocation( const char*name )   
{
	return this->mLocations[name];
}