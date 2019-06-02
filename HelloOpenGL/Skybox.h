#pragma once
#include <windows.h>
#include <gl/GL.h>
#include "texture.h"
#include "vector3f.h"

class Skybox
{
public:
	Texture*front, *back, *left, *right, *top, *bottom;
	void Init(const char*path);
	void Draw(const Vector3f camPos);
};

