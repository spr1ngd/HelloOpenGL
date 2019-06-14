#include "Particle.h"
#include "util.h"

void Particle::InitSpeed() 
{
	for (int i = 0; i < 100; i++) 
	{
		speeds[i] = Vector3f(srandf(),randf(),0);
	}
}

Particle::Particle(bool isRoot) :mIsRoot(false)
{
	mLifeTime = 6.0f + 2.0f * srandf();
	mDelayTime = 4.0f * srandf();
	mExistTime = 0.0f;
	mOriginPos = Vector3f(-400 * srandf(),-300 + 20 * randf() ,0.0f);
	mPos = mOriginPos;
	mR = 120;
	mG = 80;
	mB = 25;
	mBlendMode = GL_ONE;
	mAlpha = 0;
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
		Vector3f offset = speeds[int(99 * randf())] * deltaTime * 20;
		
		/*mPos.x += sinf(20 * deltaTime);
		mPos.y += sinf(10 * deltaTime);*/
		//printf("(%f,%f,%f)\n",offset.x,offset.y,offset.z);

		mPos = offset + mPos; 
		mExistTime += deltaTime;

		if (mExistTime > mDelayTime) // ÒÆ¶¯
		{
			mAlpha = 255;
			mR = 255 * randf();
			mG = 255 * randf();
			if (mExistTime > mLifeTime + mDelayTime)
			{
				mPos = mOriginPos;
				mExistTime = 0.0f;
			}
		}
		else
		{
			mAlpha = 0;
		}
		
	}
	if (nullptr != mNext) 
	{
		Next<Particle>()->Update(deltaTime);
	}
}