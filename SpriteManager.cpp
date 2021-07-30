#include "SpriteManager.h"
#include "Utils.h"
#include "XmlReader/tinyxml.h"

CSprites* CSprites::__instance = NULL;

CSprites* CSprites::GetInstance()
{
	if (__instance == NULL) __instance = new CSprites();
	return __instance;
}


void CSprites::Add(std::string id, int left, int top, int right, int bottom, int xPivot, int yPivot, LPDIRECT3DTEXTURE9 tex)
{
	LPSPRITE s = new CSprite(id, left, top, right, bottom, xPivot, yPivot, tex);
	sprites[id] = s;
}

LPSPRITE CSprites::Get(std::string id)
{
	return sprites[id];
}


void CSprites::Clear()
{
	for (auto x : sprites)
	{
		LPSPRITE s = x.second;
		delete s;
	}

	sprites.clear();
}


bool CSprites::LoadSpriteFromFile(std::string filePath) {

	OutputDebugStringW(ToLPCWSTR(filePath.c_str()));
	TiXmlDocument document(filePath.c_str());
	if (!document.LoadFile())
	{
		OutputDebugStringW(ToLPCWSTR(document.ErrorDesc()));
		return false;
	}

	TiXmlElement* root = document.RootElement();
	for (TiXmlElement* texture = root->FirstChildElement(); texture != nullptr; texture = texture->NextSiblingElement()) {
		string textureID = texture->Attribute("textureId");
		LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(textureID);

		if (tex != nullptr)
			OutputDebugStringW(ToLPCWSTR("Texture id: " + textureID + '\n'));

		for (TiXmlElement* node = texture->FirstChildElement(); node != nullptr; node = node->NextSiblingElement())
		{
			string spriteID = node->Attribute("id");
			int left = 0, top = 0, width = 0, height = 0, xPivot = 0, yPivot = 0;
			node->QueryIntAttribute("left", &left);
			node->QueryIntAttribute("top", &top);
			node->QueryIntAttribute("width", &width);
			node->QueryIntAttribute("height", &height);
			node->QueryIntAttribute("xPivot", &xPivot);
			node->QueryIntAttribute("yPivot", &yPivot);
			left *= 3;
			top *= 3;
			width *= 3;
			height *= 3;
			OutputDebugStringW(ToLPCWSTR(spriteID + ':' + to_string(left) + ':' + to_string(top) + ':' + to_string(width) + ':' + to_string(height) + '\n'));
			Add(spriteID, left, top, left + width, top + height, xPivot, yPivot, tex);
		}
	}
	

	return true;
}