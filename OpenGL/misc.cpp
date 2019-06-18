#include "misc.h"
#include <stdio.h>

GLuint CreateBufferObject(GLenum target, GLsizeiptr size, GLenum usage,const void* data)
{
	GLuint object;
	glGenBuffers(1, &object);
	glBindBuffer(target, object);
	glBufferData(target, size, data, usage);
	glBindBuffer(target, 0);
	return object;

}

char* LoadFileContent(const char* filePath)
{
	FILE* pFile = fopen(filePath, "rb");
	if (pFile)
	{
		fseek(pFile, 0, SEEK_END);
		int len = ftell(pFile);
		char* buffer = new char[len + 1];
		rewind(pFile);
		if (len != 0)
		{
			fread(buffer, len, 1, pFile);
			buffer[len] = '\0';
		}
		else 
		{
			printf("load file %s fail len is 0",filePath);
		}
		fclose(pFile);
		return buffer;
	}
	else
	{
		printf("load file %s fail.",filePath);
	} 
	return nullptr;
}