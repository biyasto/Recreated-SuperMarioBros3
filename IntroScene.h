#pragma once
#include "Game.h"
#include "Scene.h"
#include "GameObject.h"
#include <map>
#include <unordered_map>
#include "SelectionNode.h"
#include "IntroSceneObj.h"

class CIntroScene : public CScene
{
	int selectedOption = 0;
protected:
	LPGAMEOBJECT player; // Player controls the position of camera.
	CIntroSceneObj* cursor;

	vector<LPGAMEOBJECT> objs;


public:
	CIntroScene(std::string, std::string, std::string);
	void ChangeOption(int);
	bool GetCursor(vector<LPGAMEOBJECT>*, CIntroSceneObj**);
	virtual bool LoadDataFromFile();
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	bool isMoving = false;

};

class CIntroSceneKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states) {};
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) {};
	CIntroSceneKeyHandler(CScene* s) : CScenceKeyHandler(s) {};
};

