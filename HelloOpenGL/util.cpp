#include "util.h"

unsigned char* LoadFileContent(const char* filePath) 
{
	unsigned char* fileContent = nullptr;
	FILE* pFile = fopen(filePath,"rb");
	if (pFile) 
	{
		// read
		fseek(pFile,0,SEEK_END);
		int nLen = ftell(pFile);
		if (nLen > 0) 
		{
			rewind(pFile);
			fileContent = new unsigned char[nLen+1];
			fread(fileContent,sizeof(unsigned char),nLen,pFile);
			fileContent[nLen] = '0';
		} 
		fclose(pFile);
	}
	return fileContent;
}

GLuint CaptureScreen(int width,int height ,std::function<void()>foo) 
{
	foo();
	unsigned char * screenPixel = new unsigned char[width*height*3];
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, screenPixel);
	GLuint texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	//operation on current texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, /*GL_CLAMP*/GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, /*GL_CLAMP*/GL_REPEAT);

	// 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, screenPixel);
	glBindTexture(GL_TEXTURE_2D, 0);
	delete screenPixel;
	return texture;
}