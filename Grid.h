#pragma once
#include <string>
#include <vector>
#include "Transform.h"
#include "Cell.h"


class CGrid {
private:
	RectF viewport;
	int numOfColumns, numOfRows;
	std::vector<std::vector<LPCell>> gridCells;
	std::vector<LPCell> activeCells;

public:
	CGrid(int = 0, int = 0);
	void AddObjectToGrid(CGameObject*);
	void AddObjectToGrid(CGameObject*, int, int);

	void Update();
	LPCell GetCellByPos(float, float);
	void UpdateGridObjects();
	void UpdateObjectCellPosInGrid(CGameObject*);

	std::vector<std::vector<LPCell>> GetGridCells() { return gridCells; }

	std::vector<LPCell> GetCellsInGrid();

	std::vector<CGameObject*> GetObjectsInGrid();
};