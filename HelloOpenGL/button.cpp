#include "button.h"

Button::Button(): offsetY(0.0f),clickHandler(nullptr)
{

}

void Button::Draw() 
{
	// draw
	glPushMatrix();
	glTranslatef(0, offsetY, 0);
	mDefaultSprite->Draw();
	glPopMatrix();
	RenderableObject::Draw();
}

void Button::SetDefaultSprite(ImageSprite* sprite) 
{
	mDefaultSprite = sprite;
}	

void Button::SetRect( float x , float y , float width, float height )
{
	mXMin = x - width / 2.0f;
	mXMax = x + width / 2.0f;
	mYMin = y - width / 2.0f;
	mYMax = y + width / 2.0f;
	mDefaultSprite->SetRect(x,y,width,height);
}

void Button::Repaint()
{
	offsetY = 0.0f;
	if (nullptr != mNext) 
	{
		Next<Button>()->Repaint();
	}
}

void Button::AddClickEvent( std::function<void()> foo )   
{
	clickHandler = foo;
}

Button* Button::OnTouchBegin( int x , int y )   
{
	if (x >= mXMin && x <= mXMax && y >= mYMin && y <= mYMax) 
	{
		offsetY = -4.0f;
		return this;
	} 
	Button* btn = nullptr;
	if (nullptr != mNext) 
	{
		btn = Next<Button>()->OnTouchBegin(x,y);
	}
	return btn;
}

Button* Button::OnTouchEnd(int x, int y) 
{
	if (x >= mXMin && x <= mXMax && y >= mYMin && y <= mYMax)
	{
		if (nullptr != clickHandler)
		{
			clickHandler();
		}
		return this;
	} 
	Button* btn = nullptr;
	if (nullptr != mNext) 
	{
		btn = Next<Button>()->OnTouchEnd(x,y);
	}
	return btn;
}  