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
	unsigned char mAlpha;
	int mFadeSpeed;
	bool mbFadeIn, mbFadeOut;
	void SetTexture(Texture* texture);
	void SetRect(float x,float y ,float width,float height);
	void Draw();

	void Update(float deltaTime);
	void FadeIn(float duration);
	void FadeOut(float duration);
};

