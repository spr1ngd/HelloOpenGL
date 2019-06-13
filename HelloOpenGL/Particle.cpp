#include "Particle.h"
#include "util.h"

Particle::Particle(bool isRoot) :mIsRoot(false)
{
	mLifeTime = 6.0f + 2.0f * srandf();
	mExistTime = 0.0f;
	mOriginPos = Vector3f(-400 * srandf(),-300 + 20 * randf() ,0.0f);
	mPos = mOriginPos;
}

void Particle::Draw() 
{
	if (!mIsRoot)
	{
		ImageSprite::Draw();
	}
	if (nullptr != mNext) 
	{
		Next<Particle>()->Draw();
	}
}

void Particle::Update(float deltaTime) 
{
	if (!mIsRoot)
	{ 
		mPos.x += sinf(20 * deltaTime);
		mPos.y += sinf(10 * deltaTime);
		mExistTime += deltaTime;
		if (mExistTime > mLifeTime)
		{
			mPos = mOriginPos;
			mExistTime = 0.0f;
		}
	}
	if (nullptr != mNext) 
	{
		Next<Particle>()->Update(deltaTime);
	}
}