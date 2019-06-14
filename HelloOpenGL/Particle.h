#pragma once
#include <windows.h>
#include <gl/GL.h>
#include "imagesprite.h"
#include "Vector3f.h"

static Vector3f speeds[100];
class Particle : public ImageSprite
{
public:
	bool mIsRoot;
	float mLifeTime, mExistTime,mDelayTime;
	Vector3f mOriginPos;
	Particle(bool isRoot = false);
	void Draw();
	void Update(float deltaTime);

	static void InitSpeed();
};