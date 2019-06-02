#pragma once
#include "Vector3f.h"

class Camera
{
public:
	Camera();
	Vector3f mPos, mViewCenter, mUp;
	bool isPerspective;
	bool isMoveLeft, isMoveRight,isMoveForward,isMoveBack;
	bool isLeftRotate, isRightRotate;
	float mViewportWidth, mViewportHeight;
	
	void Update(float deltaTime);
	void RotateView(float angle, float x, float y, float z);
	void Pitch(float angle);
	void Yaw(float angle);
	void SwitchTo3D();
	void SwitchTo2D();
};

