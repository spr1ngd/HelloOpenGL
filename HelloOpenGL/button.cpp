#include "button.h"

void Button::Draw() 
{
	// draw
	mDefaultSprite->Draw();
	RenderableObject::Draw();
}

void Button::SetDefaultSprite(ImageSprite* sprite) 
{
	mDefaultSprite = sprite;
}

void Button::SetRect( float x , float y , float width, float height )
{
	mDefaultSprite->SetRect(x,y,width,height);
}

void Button::AddClickEvent( std::function<void()> foo )   
{
	clickHandler = foo;
}

Button* Button::OnTouchBegin( int x , int y )   
{

}

Button* Button::OnTouchEnd(int x, int y) 
{
	if (nullptr != clickHandler)
	{
		clickHandler();
	}
}  