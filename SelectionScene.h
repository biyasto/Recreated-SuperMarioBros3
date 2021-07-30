#pragma once
#include "Game.h"
#include "Scene.h"
#include "GameObject.h"
#include <map>
#include <unordered_map>
#include "SelectionNode.h"


class CSelectionScene : public CScene
{

protected:
	LPGAMEOBJECT player = NULL;
	vector<LPGAMEOBJECT> selectionPortals;

	std::unordered_map<std::string, CSelectionNode*> selectionNodes;
	CSelectionNode* standingNode = nullptr;

public:
	CSelectionScene(std::string, std::string, std::string);
	virtual bool LoadDataFromFile();

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	CSelectionNode* GetStandingNode() { return standingNode; }
	void ProcessMovingToNewNode(int);
	bool isMoving = false;

};

class CSelectionSceneKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states) {};
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) {};
	CSelectionSceneKeyHandler(CScene* s) : CScenceKeyHandler(s) {};
};

