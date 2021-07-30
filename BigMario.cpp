#include "BigMario.h"

CBigMario::CBigMario(float x, float y) : CMario(x, y) {
	type = MarioType::TALL;
}

void CBigMario::Render(Vector2 finalPos)
{
	CMario::Render(finalPos);
}
