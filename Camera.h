 #pragma once
#include "Transform.h"
#include "BuildWorld.h"
#include "GameObject.h"

struct MapData
{
	int timeRemaining = 0;
	int score = 0;
	int playerLife = 1;
};

struct SVibrating {
	DWORD totalTime = 0;
	DWORD beginTime = 0;
	Vector2 totalDeltaPosVibrating = Vector2(50, 0);
	DWORD timeOfEachSide = 40;
};

class CCamera {

protected:
	SVibrating vibrating;
	Vector2 camPosition ;
	Vector2 camSize; // (width,height) px
	Vector2 mapSize; // (width, height) px
	Vector2 deltaPos = Vector2(0, 0);
	Vector2 LeftTopLimit = Vector2(0, 0);
	Vector2 RightBottomLimit = Vector2(0, 0);

	bool isLocked = true;

	CGameObject* positionController; // who controls the camera position
	CGameObject* mario;
	CGameBuildWorld* mMap;
	MapData	mapData;
	

public:

	CCamera();
	~CCamera();
	Vector2 GetCamPosition();
	Vector2 GetCamSize() { return camSize; }
	Vector2 GetMapSize() { return mapSize; }
	LPGAMEOBJECT GetPositionController() { return positionController; }
	void SetCamPosition(Vector2 pos);
	Vector2 ConvertPosition(Vector2 pos);
	
	void BeginVibrating(DWORD = 200);

	void ChangeCamPosition(Vector2);
	void ChangeCamArea(Vector2 ar1, Vector2 ar2);

	void SetMapData(int=-1, int=-1, int=-1);
	void AdjustTimeRemaining(int);
	void AdjustScore(int);

	void RenderPausing();
	void RenderFinishPlayScene();

	bool IsInCamera(Vector2 realPos, int outsideCam = 200);
	
	void InitPositionController(CGameObject* a);
	void InitMario(CGameObject* );
	void UpdateCamPosition();
	
	CGameBuildWorld* GetMap() { return mMap; }
	
	// IntroScene
	void LoadMap(std::string, vector<LPGAMEOBJECT>* = NULL);
	// PlayScene
	void LoadMap(std::string, LPGAMEOBJECT* = NULL, vector<LPGAMEOBJECT>* = NULL);
	// SelectionScene
	void LoadMap(std::string, vector<LPGAMEOBJECT>*, std::unordered_map<std::string, CSelectionNode*>* );
	

	void Update(DWORD dt);
	void Render();
	void RenderDetailBoard();
};