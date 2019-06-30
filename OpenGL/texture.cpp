
#include "soil.h"
#include "texture.h"
#include "util.h"

#pragma comment(lib,"soil.lib")
unsigned char* DecodeBMP( unsigned char*bmpFileData ,int&width,int&height) 
{
	if (0x4D42 == *((unsigned short*)bmpFileData))
	{
		int pixelDataOffset = *((int*)(bmpFileData + 10));
		width = *(int*)(bmpFileData + 18);
		height = *(int*)(bmpFileData + 22);
		unsigned char* pixelData = bmpFileData + pixelDataOffset;
		// bgr bgr bgr....
		// rgb rgb rgb
		for (int i = 0; i < width * height * 3; i += 3) 
		{
			unsigned char temp = pixelData[i];
			pixelData[i] = pixelData[i + 2];
			pixelData[i + 2] = temp;
		}
		return pixelData;
	}
	else 
	{
		return nullptr;
	}
} 

void Texture::Init(const char*imagePath,bool invertY,GLenum warpMode ,bool isSubImage) 
{
	unsigned int flags = SOIL_FLAG_POWER_OF_TWO;
	if (invertY)
		flags |= SOIL_FLAG_INVERT_Y;
	mPath = imagePath;
	mTextureID = SOIL_load_OGL_texture(imagePath,0,0, flags);
	if (warpMode == GL_CLAMP_TO_EDGE) 
	{
		glBindTexture(GL_TEXTURE_2D, mTextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, warpMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, warpMode);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	return;
	unsigned char* imageFileContent = (unsigned char*)LoadFileContent(imagePath);
	// decode image
	int width = 0, height = 0;
	unsigned char* pixelData = DecodeBMP(imageFileContent, width, height);
	// generate an opengl texture
	glGenTextures(1,&mTextureID);
	glBindTexture(GL_TEXTURE_2D,mTextureID);
	//operation on current texture
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, /*GL_CLAMP*/GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, /*GL_CLAMP*/GL_REPEAT);
	
	// 
	if (isSubImage)
	{
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixelData);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixelData);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	delete imageFileContent;
}

std::unordered_map<std::string, Texture*> Texture::mCachedTextures;
Texture* Texture::LoadTexture( const char*imagePath ,bool invertY ,GLenum warpMode ,bool isSubImage )   
{
	if (mCachedTextures.find(imagePath) != mCachedTextures.end())
	{
		return mCachedTextures[imagePath];
	}
	Texture* texture = new Texture;
	texture->Init(imagePath,invertY, warpMode);
	printf("load texture: %s\n ",imagePath);
	mCachedTextures.insert(std::pair<std::string,Texture*>(imagePath,texture));
	return texture;
}

void Texture::UnLoadTexture(Texture*texture)  
{
	auto iter = mCachedTextures.find(texture->mPath.c_str());
	if (iter != mCachedTextures.end()) 
	{
		mCachedTextures.erase(iter);
		glDeleteTextures(1,&iter->second->mTextureID);
		delete texture;
	}
}