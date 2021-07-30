#pragma once
#include <unordered_map>
#include <d3dx9.h>


/*
	Manage texture database
*/
class CTextures
{
	static CTextures* __instance;

	std::unordered_map<std::string, LPDIRECT3DTEXTURE9> textures;

public:
	CTextures();
	void Add(std::string id, LPCWSTR filePath, D3DCOLOR transparentColor);
	LPDIRECT3DTEXTURE9 Get(std::string i);

	bool LoadTextureFromFile(LPCWSTR filePath);

	void Clear();
	static CTextures* GetInstance();
};