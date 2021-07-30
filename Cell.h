#pragma once
#include <unordered_set>
#include "GameObject.h"
#include "RectF.h"

class CCell
{
private:

	Vector2 positionInGrid;
	std::unordered_set<CGameObject*> objects;

public:

	static int cellWidth, cellHeight;

	CCell(Vector2);

	void AddObject(CGameObject*);

	void RemoveObject(CGameObject*);

	RectF GetViewport();

	std::unordered_set<CGameObject*> GetObjects() { return this->objects; }
};
