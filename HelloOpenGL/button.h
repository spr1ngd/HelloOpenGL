#pragma once
#include "list.h"
#include "imagesprite.h"
#include <functional>

class Button : public RenderableObject
{
public:
	ImageSprite* mDefaultSprite;
	std::function<void()> clickHandler;
	void SetDefaultSprite(ImageSprite* sprite);
	void SetRect(float x, float y , float width, float height);
	void Draw();

	void AddClickEvent(std::function<void()> foo);
	Button* OnTouchBegin(int x ,int y);
	Button* OnTouchEnd(int x, int y);
};

