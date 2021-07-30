#pragma once
#include "Utils.h"
#include "TileSet.h"
#include "MapLayer.h"
#include "GameObject.h"
#include <unordered_map>
#include "SelectionNode.h"
#include "Grid.h"
#include "./XmlReader/tinyxml.h"
#include "./XmlReader/tinystr.h"

struct kTilesetRender {
	CTileSet* tileset;
	Vector2 rangeId = Vector2(0, 0);
};

class CGameBuildWorld
{
	Vector2 camPosition;
	CGrid grid;

	int width;
	int height;
	int tileWidth;
	int tileHeight;

	vector<CTileSet*> tilesets;
	kTilesetRender kRender;
	vector<CMapLayer*> layers;

public:
	CGameBuildWorld();
	~CGameBuildWorld();
	virtual void Update(int dt);

	virtual Vector2 GetBound();
	virtual CTileSet* GetTileSetByTileID(int id);
	virtual void AddTileSet(int firstgid, CTileSet* tileSet);

	virtual void AddLayer(CMapLayer* layer);
	virtual void Render(float bottomMargin = 0);

	void UpdateCamPosition(Vector2 newPos);
	Vector2 ConvertToPositionInCam(Vector2 oldPos);

	virtual void GetMapSize(Vector2& out);
	CGrid* GetGrid() { return &grid; }
	virtual Vector2 GetMapSize();

	// PlayScene
	static CGameBuildWorld* FromTMX(std::string, LPGAMEOBJECT*, vector<LPGAMEOBJECT>*);
	// SelectionScene
	static CGameBuildWorld* FromTMX(std::string, vector<LPGAMEOBJECT>*, std::unordered_map<std::string, CSelectionNode*>*);
	// IntroScene
	static CGameBuildWorld* FromTMX(std::string, vector<LPGAMEOBJECT>*);

	
};

typedef CGameBuildWorld* LPGameMap;
