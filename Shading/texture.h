#pragma once
#include <windows.h>
#include <gl/GL.h>
#include <string>
#include <unordered_map>

#define GL_CLAMP_TO_EDGE 0x812f

class Texture
{
public:
	GLuint mTextureID; // gpu
	std::string mPath;
	void Init(const char*imagePath, bool invertY, GLenum warpMode = GL_CLAMP_TO_EDGE,bool isSubImage = false);
	static Texture* LoadTexture(const char*imagePath,bool invertY = true,GLenum warpMode = GL_CLAMP_TO_EDGE,bool isSubImage = false);
	static void UnLoadTexture(Texture* texture);
	static std::unordered_map<std::string, Texture*> mCachedTextures;

	// skybox
	static unsigned int LoadSkyboxTextures(const char*texturePath);
};

