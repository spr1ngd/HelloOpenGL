#pragma once
#include <windows.h>
#include <gl/GL.h>
#include "imagesprite.h"
#include "Vector3f.h"

class Particle : public ImageSprite
{
public:
	bool mIsRoot;
	float mLifeTime, mExistTime;
	Vector3f mOriginPos;
	Particle(bool isRoot = false);
	void Draw();
	void Update(float deltaTime);
};