#include "fullscreenquad.h"

void FullScreenQuad::Init() 
{
	float pos[] =
	{
		-0.5f,-0.5f,0.0f,
		0.5f,-0.5f,0.0f,
		0.5f,0.5f,0.0f,
		-0.5f,0.5f,0.0f
	};
	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, pos, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void FullScreenQuad::Draw(GLuint posLocation)
{
	//glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glEnableVertexAttribArray(posLocation);
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glDrawArrays(GL_QUADS, 0, 4);
	//glBindBuffer(GL_ARRAY_BUFFER,0);
}

//void FullScreenQuad::Draw(GLuint posLocation,Rect* rect)
//{
//	float pos[] =
//	{
//		rect->left, rect->bottom,0.0f,
//		rect->right,rect->bottom,0.0f,
//		rect->right,rect->top,0.0f,
//		rect->left, rect->top,0.0f
//	};
//
//	glGenBuffers(1,&mVBO);
//	glBindBuffer(GL_ARRAY_BUFFER,mVBO);
//	glBufferData(GL_ARRAY_BUFFER,sizeof(float)*12,pos,GL_STATIC_DRAW);
//	glBindBuffer(GL_ARRAY_BUFFER,0);
//
//	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
//	glEnableVertexAttribArray(posLocation);
//	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
//	glDrawArrays(GL_QUADS,0,4);
//	glBindBuffer(GL_ARRAY_BUFFER,0);
//	delete rect;
//}