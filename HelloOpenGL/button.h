#pragma once
#include "list.h"
#include "imagesprite.h"
#include <functional>

class Button : public RenderableObject
{
public:
	Button();
	ImageSprite* mDefaultSprite;
	float mXMin, mXMax, mYMin, mYMax;
	float offsetY;
	std::function<void()> clickHandler;
	void SetDefaultSprite(ImageSprite* sprite);
	void SetRect(float x, float y , float width, float height);
	void Draw();
	void Repaint();

	void AddClickEvent(std::function<void()> foo);
	Button* OnTouchBegin(int x ,int y);
	Button* OnTouchEnd(int x, int y);
};

