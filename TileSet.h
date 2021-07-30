#pragma once
#include "Utils.h"
#include "Transform.h"
#include "./XmlReader/tinyxml.h"
#include "./XmlReader/tinystr.h"
#include "RectF.h"

#include <unordered_map>


class CTileSet
{
	int firstgid;
	
	D3DXVECTOR2 tileSize;

	int tileCount;
	int columns;
	std::string name;

	LPDIRECT3DTEXTURE9 texture;

	

public:
	CTileSet(int firstgid, Vector2 tileSize, int tileCount, int columns, string imgPath);
	CTileSet(TiXmlElement* data, string xmlPath);

	Vector2 GetRangeId();

	

	void Draw(int gid, Vector2 finalPos);

	~CTileSet();
};

typedef CTileSet* LPTileSet;

