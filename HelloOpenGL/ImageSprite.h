#pragma once
#include "texture.h"
#include "Vector3f.h"
#include "list.h"

class ImageSprite : public RenderableObject
{
public:
	ImageSprite();
	Texture* mTexture;
	Vector3f mMesh[4];
	unsigned char mR, mG, mB,mAlpha;
	GLenum mBlendMode;
	Vector3f mPos;
	void SetTexture(Texture* texture);
	void SetRect(float x,float y ,float width,float height);
	virtual void Draw();

	int mFadeSpeed;
	bool mbFadeIn, mbFadeOut;
	virtual void Update(float deltaTime);
	void FadeIn(float duration);
	void FadeOut(float duration);
};