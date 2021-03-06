#include "fullscreenquad.h"

void FullScreenQuad::Init() 
{
	
}

void FullScreenQuad::Draw(GLuint posLocation,GLuint texcoordLocation)
{
	float pos[] =
	{
		-1.0f,-1.0f,0.0f ,0.0f,0.0f,
		1.0f ,-1.0f,0.0f ,1.0f,0.0f,
		1.0f ,1.0f ,0.0f ,1.0f,1.0f,
		-1.0f,1.0f ,0.0f ,0.0f,1.0f
	};
	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 20, pos, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glEnableVertexAttribArray(posLocation);
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
	glEnableVertexAttribArray(texcoordLocation);
	glVertexAttribPointer(texcoordLocation,2,GL_FLOAT,GL_FALSE,sizeof(float) * 5,(void*)(sizeof(float)*3));
	glDrawArrays(GL_QUADS, 0, 4);
	glBindBuffer(GL_ARRAY_BUFFER,0);
}

void FullScreenQuad::Draw(GLuint posLocation, GLuint texcoordLocation,Rect* rect = nullptr)
{
	float pos[] =
	{
		rect->left, rect->bottom,0.0f,0.0f,0.0f,
		rect->right,rect->bottom,0.0f,1.0f,0.0f,
		rect->right,rect->top,   0.0f,1.0f,1.0f,
		rect->left, rect->top,   0.0f,0.0f,1.0f
	};

	glGenBuffers(1,&mVBO);
	glBindBuffer(GL_ARRAY_BUFFER,mVBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(float)*20,pos,GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,0);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glEnableVertexAttribArray(posLocation);
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
	glEnableVertexAttribArray(texcoordLocation);
	glVertexAttribPointer(texcoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));
	glDrawArrays(GL_QUADS,0,4);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	delete rect;
}