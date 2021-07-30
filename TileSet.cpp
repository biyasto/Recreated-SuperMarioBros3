#include "TileSet.h"
#include "Game.h"
#include "TextureManager.h"

CTileSet::CTileSet(int firstgid, Vector2 tileSize, int tileCount, int columns, string imgPath)
{
	this->firstgid = firstgid;
	this->tileSize = Vector2(tileSize.x, tileSize.y);
	this->tileCount = tileCount;
	this->columns = columns;
	
}

CTileSet::CTileSet(TiXmlElement* data, string xmlPath)
{
	data->QueryIntAttribute("firstgid", &this->firstgid);
	data->QueryFloatAttribute("tilewidth", &this->tileSize.x);
	data->QueryFloatAttribute("tileheight", &this->tileSize.y);
	data->QueryIntAttribute("tilecount", &this->tileCount);
	data->QueryIntAttribute("columns", &this->columns);
	
	this->name = data->Attribute("name");
	
	TiXmlElement* imgDom = data->FirstChildElement("image");
	string imgPath = imgDom->Attribute("source");
	CTextures::GetInstance()->Add("tileset_" + this->name, ToLPCWSTR("Resources/"+ name + "/" +imgPath), D3DCOLOR_ARGB(0, 0, 0, 0));
	this->texture = CTextures::GetInstance()->Get("tileset_" + this->name);


	
}

Vector2 CTileSet::GetRangeId()
{
	return Vector2(firstgid, firstgid+ tileCount);
}



void CTileSet::Draw(int gid, Vector2 finalPos)
{
	
	if (gid < firstgid) return;
	RECT r;
	r.top = ((gid - firstgid) / columns) * tileSize.y;
	r.left = ((gid - firstgid) % columns) * tileSize.x;
	r.bottom = r.top + tileSize.y;
	r.right = r.left + tileSize.x;

	CGame::GetInstance()->DrawWithScaling(finalPos, Vector2(0, 0), texture, r, 255);

}

CTileSet::~CTileSet()
{
	if (texture) {
		delete texture;
	}
	texture = NULL;
}
