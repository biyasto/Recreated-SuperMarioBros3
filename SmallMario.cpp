#include "SmallMario.h"

CSmallMario::CSmallMario(float x, float y) : CMario(x, y) {
	type = MarioType::SMALL;
}

void CSmallMario::Render(Vector2 finalPos)
{
	CMario::Render(finalPos);

}

