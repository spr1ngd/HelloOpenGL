#include "GPUProgram.h"

using namespace spring;

GPUProgram GPUProgram::createProgram() 
{
	GPUProgram program;
	GLuint programId = glCreateProgram();
	return program;
}