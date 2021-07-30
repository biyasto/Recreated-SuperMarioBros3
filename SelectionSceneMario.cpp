#include "SelectionSceneMario.h"
#include "Animation.h"
using namespace std;

CSelectionSceneMario::CSelectionSceneMario(Vector2 pos) {
	this->x = pos.x;
	this->y = pos.y;
}

void CSelectionSceneMario::Render(Vector2 finalPos)
{
	string aniId = "ani-big-mario-selection";
	CAnimations::GetInstance()->Get(aniId)->Render(finalPos);
}
