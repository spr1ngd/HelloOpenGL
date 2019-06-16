#include "fontmanager.h"
#include <unordered_map>

HDC DC = nullptr;
GLuint text;
std::unordered_map<wchar_t, GLuint> cachedChars;

void FontManager::Init(HDC dc) 
{
	DC = dc;
	HFONT hFont = CreateFont(24,
		0,
		0,
		0,
		FW_NORMAL,
		FALSE,
		FALSE,
		FALSE,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		"ËÎÌו");
	SelectObject(dc, hFont);
}

void FontManager::Draw(const wchar_t* str )
{
	glDisable(GL_LIGHTING);
	int len = wcslen(str);
	for (int i = 0; i < len; i++) 
	{
		auto iter = cachedChars.find(str[i]);
		if (iter == cachedChars.end())
		{
			text = glGenLists(1);
			wglUseFontBitmaps(DC, str[i], 1, text);
			cachedChars.insert(std::pair<wchar_t,GLuint>(str[i],text));
		}
		glCallList(cachedChars[str[i]]);
	}
}