#pragma once
#include "Game.h"
#include "TextureManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Mario.h"
#include "Effect.h"
#include <map>

//#include "GameMap2.h"

struct SChangeCamData {
	Vector2 playerPos; 
	Vector2 LeftTopLimit;
	Vector2 RightBottomLimit;
};

class CPlayScene : public CScene
{

protected:
	bool isPausing = false;
	LPGAMEOBJECT player; // Player controls the position of camera.
	int playerLevel = 1;
	//shared_ptr<CGameMap> kMap;
	vector<LPGAMEOBJECT> staticObjects;
	vector<LPGAMEOBJECT> mainObjects;
	vector<LPGAMEOBJECT> enemyBullets;
	vector<CEffect*> effects;

	LPGAMEOBJECT cameraLimitController;



	float standingY;
	SChangeCamData changeCamData;


public:
	CPlayScene(std::string id, std::string, std::string);
	void SwitchPlayer(LPGAMEOBJECT newPlayer);
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	MarioType GetMarioType();

	virtual bool LoadDataFromFile();

	void SetPlayerLevel(int a) { playerLevel = a; }

	vector<LPGAMEOBJECT> GetEnemyBullets() { return enemyBullets; }
	bool GetPausingStatus() { return isPausing; }
	void TogglePausingMode();

	LPGAMEOBJECT GetPlayer() { return player; }
	void SetPlayer(LPGAMEOBJECT obj) { player = obj; }

	void PushDynamicObjects(LPGAMEOBJECT obj) { obj->AddPriority(EPriorityFlag::DYNAMIC_OBJECT); }
	void PushTempObjects(LPGAMEOBJECT obj) { obj->AddPriority(EPriorityFlag::TEMP_OBJECT); }
	void PushHighPriorityObjects(LPGAMEOBJECT obj) { obj->AddPriority(EPriorityFlag::HIGH_PRIORITY_OBJECT); }
	void PushEffects(CEffect* eff) { effects.push_back(eff); }
	void PushEnemyBullet(LPGAMEOBJECT obj) { enemyBullets.push_back(obj); }

	void HandleSceneTime(DWORD&);

	void UpdateEffects(DWORD);

	void DownLevelMario();
	void UpLevelMario();

	void HandleMarioDie();

	void SwitchToSelectionScene();

	CMario* GenerateMario(MarioType, Vector2);

	void PrepareChangeCameraArea(Vector2, Vector2, Vector2);
	void ChangeCameraArea();
	void BeginVibratingCamera(DWORD = 200);
	void ChangeGoldenBricksInCameraToShowCoinState();
	


};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};

