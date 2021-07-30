#pragma once
#include "Mario.h"

class CBigMario : public CMario
{
	
public:
	CBigMario(float x = 0.0f, float y = 0.0f);
	void Render(Vector2 finalPos);

};

