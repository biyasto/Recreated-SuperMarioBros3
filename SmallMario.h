#pragma once
#include "Mario.h"

class CSmallMario : public CMario
{

public:
	CSmallMario(float x = 0.0f, float y = 0.0f);
	void Render(Vector2 finalPos);
};

