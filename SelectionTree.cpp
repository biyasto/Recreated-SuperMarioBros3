#include "SelectionTree.h"
#include "Animation.h"

CSelectionTree::CSelectionTree(Vector2 pos) {
	this->x = pos.x;
	this->y = pos.y;
}

void CSelectionTree::Render(Vector2 finalPos)
{
	std::string aniId = "ani-tree";
	CAnimations::GetInstance()->Get(aniId)->Render(finalPos);
}
