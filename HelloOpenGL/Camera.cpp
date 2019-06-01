#include "camera.h"
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <stdio.h>

Camera::Camera() :mPos(0.0f, 0.0f, 0.0f),
mViewCenter(0.0f, 0.0f, -1.0f),
mUp(0.0f, 1.0f, 0.0f),
isMoveLeft(false),
isMoveRight(false),
isMoveForward(false),
isMoveBack(false),
isLeftRotate(false),
isRightRotate(false)
{

}

void Camera::RotateView(float angle, float x, float y, float z) 
{
	Vector3f viewDirection = mViewCenter - mPos;
	//viewDirection.Normalize();
	
	float C = cosf(angle);
	float S = sinf(angle);
	float mC = 1 - C;
	float mS = 1 - S;

	Vector3f tempX(C + x * x * mC, x * y * mC - z * S, x * z * mC + y * S);
	Vector3f tempY(x * y * mC + z * S, C + y * y * mC, y * z * mC - x * S);
	Vector3f tempZ(x * z * mC - y * S, y * z * mC + x * S, C + z * z * mC);
	Vector3f newDirection;
	newDirection.x = tempX * viewDirection;
	newDirection.y = tempY * viewDirection;
	newDirection.z = tempZ * viewDirection;
	mViewCenter = mPos + newDirection;
}

void Camera::Pitch( float angle )   
{
	Vector3f viewDirection = mViewCenter - mPos;
	viewDirection.Normalize();
	Vector3f rightDirection = viewDirection ^ mUp;
	rightDirection.Normalize();
	RotateView(angle,rightDirection.x, rightDirection.y, rightDirection.z);
}

void Camera::Yaw( float angle )   
{
	RotateView(angle, mUp.x, mUp.y, mUp.z);
}

void Camera::Update(float deltaTime)
{ 
	float moveSpeed = 10.0f; 
	float rotateSpeed = 5.0f;
	 
	if (isMoveLeft)
	{
		//Vector3f leftDirection(-1.0f, 0.0f, 0.0f);
		Vector3f viewDirection = mViewCenter - mPos;
		viewDirection.Normalize();
		Vector3f rightDirection = viewDirection ^ mUp;
		rightDirection.Normalize();

		Vector3f leftDirection = Vector3f(-rightDirection.x, -rightDirection.y, -rightDirection.z);
		// todo: 为什么交换之后不会报错
		mPos = leftDirection * moveSpeed * deltaTime + mPos;
		mViewCenter = leftDirection * moveSpeed * deltaTime + mViewCenter;
	}
	if (isMoveRight)
	{
		Vector3f viewDirection = mViewCenter - mPos;
		viewDirection.Normalize();
		Vector3f rightDirection = viewDirection ^ mUp;
		rightDirection.Normalize(); 
		mPos = rightDirection * moveSpeed * deltaTime + mPos;
		mViewCenter = rightDirection * moveSpeed * deltaTime + mViewCenter;
	}
	if (isMoveForward)
	{
		Vector3f forwardDirection = mViewCenter - mPos;
		forwardDirection.Normalize();
		mPos = forwardDirection * moveSpeed * deltaTime + mPos;
		mViewCenter = forwardDirection * moveSpeed * deltaTime + mViewCenter;
	}
	if (isMoveBack)
	{
		Vector3f backwardDirection = mPos - mViewCenter ;
		backwardDirection.Normalize();
		mPos = backwardDirection * moveSpeed * deltaTime + mPos;
		mViewCenter = backwardDirection * moveSpeed * deltaTime + mViewCenter;
	}
	//set model view matrix 
	gluLookAt(mPos.x, mPos.y, mPos.z,
		mViewCenter.x, mViewCenter.y, mViewCenter.z,
		mUp.x, mUp.y, mUp.z);
}