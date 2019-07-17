#include "FBO.h"

FBO::FBO()
{
	glGenFramebuffers(1, &mFBO);
}

void FBO::AttachColorBuffer(const char* bufferName, GLenum attachment, GLenum dataType, int width, int height)
{
	GLuint colorBuffer;
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glGenTextures(1, &colorBuffer);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, dataType, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, colorBuffer, 0);

	mDrawBuffers.push(attachment);
	mBuffer.insert(std::pair<std::string, GLuint>(bufferName, colorBuffer));
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::AttachDepthBuffer(const char* bufferName, int width, int height)
{
	GLuint depthMap;
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	mBuffer.insert(std::pair<std::string, GLuint>(bufferName, depthMap));
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::Finish() 
{
	int nCount = (int)mDrawBuffers.size();
	if (nCount > 0)
	{
		GLenum* buffers = new GLenum[nCount];
		int i = 0;
		while (!mDrawBuffers.empty())
		{
			buffers[i++] = mDrawBuffers.top();
			mDrawBuffers.pop();
		}
		glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
		glDrawBuffers(nCount, buffers);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void FBO::Bind() 
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
}

void FBO::Unbind() 
{
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}

GLuint FBO::GetBuffer(const char* bufferName)
{
	auto iter = mBuffer.find(bufferName);
	if (iter != mBuffer.end())
	{
		return iter->second;
	}
	return 0;
}