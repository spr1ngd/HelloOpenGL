#include "ImageSprite.h"

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
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, mTexture->mTextureID);
	glBegin(GL_QUADS);

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
