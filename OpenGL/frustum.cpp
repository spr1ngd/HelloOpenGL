#include "frustum.h"
#include "misc.h"

Frustum::Frustum() 
{

}

void Frustum::InitProgram()
{
	mProgram = CreateGPUProgram("res/shader/unlitcolor.vs","res/shader/unlitcolor.fs");
	mVertexLocation = glGetAttribLocation(mProgram, "vertex");
	mMLocation = glGetUniformLocation(mProgram,"M");
	mVLocation = glGetUniformLocation(mProgram,"V");
	mPLocation = glGetUniformLocation(mProgram,"P");
}

void Frustum::InitPerspective(float fov,float aspect,float zNear,float zFar) 
{
	// NEAR
	float nearX = 0.0f;
	float nearY = 0.0f;
	float tanHalfFOV = tan(fov / 2.0f);
	nearY = tanHalfFOV * zNear;
	nearX = aspect * nearY;

	// FAR
	float farX = tanHalfFOV * zFar;
	float farY = aspect * farX;
	float vertices[] =
	{
		-nearX,-nearY,-zNear,
		nearX,-nearY,-zNear,
		nearX,nearY,-zNear,
		-nearX,nearY,-zNear,
		-farX,-farY,-zFar,
		farX,-farY,-zFar,
		farX,farY,-zFar,
		-farX,farY,-zFar,
	};
	mVBO = CreateBufferObject(GL_ARRAY_BUFFER, 24, GL_STATIC_DRAW, vertices);
	unsigned int indices[] = 
	{
		0,1,1,2,2,3,3,0,
		4,5,5,6,6,7,7,4,
		0,4,1,5,2,6,3,7
	};
	mIBO = CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, 24, GL_STATIC_DRAW, indices);
}

void Frustum::Draw( float*model,float*view,float*projection )   
{
	glUseProgram(mProgram);
	glUniformMatrix4fv(mMLocation,1,GL_FALSE,model);
	glUniformMatrix4fv(mVLocation,1,GL_FALSE,view);
	glUniformMatrix4fv(mPLocation,1,GL_FALSE,projection);
	
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glEnableVertexAttribArray(mVertexLocation);
	glVertexAttribPointer(mVertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER,0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mIBO);
	glDrawElements(GL_LINE,24,GL_UNSIGNED_INT,(void*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glUseProgram(0);
}