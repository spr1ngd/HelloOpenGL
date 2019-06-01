#include "Skybox.h"

void Skybox::Init( const char* path)
{
	char temp[128] = { 0 };
	strcpy(temp, path);
	strcat(temp,"/front.bmp");
	front = Texture::LoadTexture(temp);

	strcpy(temp, path);
	strcat(temp, "/back.bmp");
	back = Texture::LoadTexture(temp);

	strcpy(temp, path);
	strcat(temp, "/right.bmp");
	right = Texture::LoadTexture(temp);

	strcpy(temp, path);
	strcat(temp, "/left.bmp");
	left = Texture::LoadTexture(temp);

	strcpy(temp, path);
	strcat(temp, "/top.bmp");
	top = Texture::LoadTexture(temp);

	strcpy(temp, path);
	strcat(temp, "/bottom.bmp");
	bottom = Texture::LoadTexture(temp);
}

void Skybox::Draw( const Vector3f camPos ) 
{
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glPushMatrix();
	glTranslatef(camPos.x,camPos.y,camPos.z);

	glColor4ub(255, 255, 255, 255);
	// front
	glBindTexture(GL_TEXTURE_2D, front->mTextureID);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f,0.0f);
	glVertex3f(-0.5f,-0.5f,-0.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glEnd();
	// back
	glBindTexture(GL_TEXTURE_2D, back->mTextureID);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glEnd();
	// left
	glBindTexture(GL_TEXTURE_2D, left->mTextureID);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glEnd();
	// right
	glBindTexture(GL_TEXTURE_2D, right->mTextureID);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glEnd();
	// top 
	glBindTexture(GL_TEXTURE_2D, top->mTextureID);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glEnd();
	// bottom
	glBindTexture(GL_TEXTURE_2D, bottom->mTextureID);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.5f, -0.5f, -0.5f);

	glEnd();
	glPopMatrix();
}