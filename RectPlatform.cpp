#include "RectPlatform.h"


CRectPlatform::CRectPlatform(float x, float y, float width, float height) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

void CRectPlatform::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - width / 2;
	top = y - height / 2;
	right = x + width / 2;
	bottom = y + height / 2;

}

void CRectPlatform::Render(Vector2 finalPos) {

}

