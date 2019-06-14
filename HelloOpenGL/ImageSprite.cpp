#include "ImageSprite.h"

ImageSprite::ImageSprite() : mAlpha(255),mbFadeIn(false),mbFadeOut(false),mFadeSpeed(255)
{
	mR = 255;
	mG = 255;
	mB = 255;
	mBlendMode = GL_ONE_MINUS_SRC_ALPHA;
}

void ImageSprite::SetTexture(Texture* texture) 
{
	mTexture = texture;
}

void ImageSprite::SetRect(float x, float y, float width, float height) 
{
	float hW = width / 2.0f;
	float hH = height / 2.0f;
	mMesh[0].x = x - hW;
	mMesh[0].y = y - hH;

	mMesh[1].x = x + hW;
	mMesh[1].y = y - hH;

	mMesh[2].x = x + hW;
	mMesh[2].y = y + hH;

	mMesh[3].x = x - hW;
	mMesh[3].y = y + hH;
}

void ImageSprite::Draw() 
{
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, mBlendMode);
	glPushMatrix();
	glTranslatef(mPos.x, mPos.y, mPos.z);
	glBindTexture(GL_TEXTURE_2D, mTexture->mTextureID);
	glBegin(GL_QUADS);

	glColor4ub(mR,mG,mB,mAlpha);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(mMesh[0].v);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3fv(mMesh[1].v);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3fv(mMesh[2].v);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3fv(mMesh[3].v);

	glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);
}

void ImageSprite::Update(float deltaTime) 
{
	if (mbFadeIn) 
	{
		int alpha = mAlpha;
		alpha -= mFadeSpeed * deltaTime; 
		mAlpha = alpha < 0.0f ? 0.0f : alpha;
		if (mAlpha == 0.0f)
		{
			mbFadeIn = false; 
		}
	}
	if (mbFadeOut)
	{
		int alpha = mAlpha;
		alpha += mFadeSpeed * deltaTime;
		mAlpha = alpha > 255.0f ? 255.0f : alpha;
		if (mAlpha == 255.0f)
		{
			mbFadeOut = false; 
		}
	}
	//printf("FADE VALUE : %d\n",mAlpha);
}

void ImageSprite::FadeIn(float duration) 
{
	if (mbFadeIn || mbFadeOut)
		return; 
	mbFadeIn = true;
	mFadeSpeed = int(255.0f / duration);
}

void ImageSprite::FadeOut(float duration) 
{
	if (mbFadeIn || mbFadeOut)
		return;
	mbFadeOut = true; 
	mFadeSpeed = int(255.0f / duration);
}