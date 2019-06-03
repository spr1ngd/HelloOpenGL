#include "Ground.h"

void Ground::Init() 
{
	mGround = glGenLists(1);
	glNewList(mGround, GL_COMPILE);
	glPushMatrix();
	glBegin(GL_QUADS); 

	for (int x = -100; x < 100; x += 10) 
	{
		for (int z = -100; z < 100; z+=10)
		{
			if (((z/10) % 2) ^ ((x/10) % 2))
			{
				glColor4ub(20,20,20,255);
			}
			else 
			{
				glColor4ub(200, 200, 200, 255);
			}
			glVertex3f(x, -1.0f, z);
			glVertex3f(x, -1.0f, z + 10);
			glVertex3f(x + 10, -1.0f, z + 10);
			glVertex3f(x + 10, -1.0f, z);
		}
	}

	glEnd(); 
	glPopMatrix();
	glEndList();
}

void Ground::Draw() 
{
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D); 
	glCallList(mGround);
}