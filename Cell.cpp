#include "Cell.h"
#include "Const.h"

int CCell::cellWidth = SCREEN_WIDTH / 2;
int CCell::cellHeight = SCREEN_HEIGHT / 2;

CCell::CCell(Vector2 pos) {
	this->positionInGrid = pos;
}

void CCell::AddObject(CGameObject* obj) {
	objects.insert(obj);
}

void CCell::RemoveObject(CGameObject* obj) {
	if (objects.find(obj) != objects.end()) {
		objects.erase(obj);
	}
}

RectF CCell::GetViewport() {
	RectF viewport = RectF(positionInGrid.x * cellWidth, positionInGrid.y * cellHeight, cellWidth, cellHeight);
	return viewport;
}