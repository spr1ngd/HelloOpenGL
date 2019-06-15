#pragma once
#include <windows.h>
#include <gl/GL.h>

class FontManager
{
public:
	static void Init(HDC dc);
	static void Draw(const wchar_t* str);
};

