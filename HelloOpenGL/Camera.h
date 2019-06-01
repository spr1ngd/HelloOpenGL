#pragma once
#include "Vector3f.h"

class Camera
{
public:
	Camera();
	Vector3f mPos, mViewCenter, mUp;
	bool isMoveLeft, isMoveRight,isMoveForward,isMoveBack;
	bool isLeftRotate, isRightRotate;
	
	void Update(float deltaTime);
	void RotateView(float angle, float x, float y, float z);
	void Pitch(float angle);
	void Yaw(float angle);
};

