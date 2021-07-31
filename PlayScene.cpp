
#include <iostream>
#include <fstream>

#include "PlayScene.h"
#include "Utils.h"
#include "TextureManager.h"
#include "Animation.h"

#include "Const.h"
#include "XmlReader/tinyxml.h"

#include "SmallMario.h"
#include "BigMario.h"
#include "TailMario.h"
#include "FireMario.h"
#include "GoldenBrick.h"

#define VELOCITY_X_ENEMY_FIRE_BULLET			0.3625f 


CPlayScene::CPlayScene(std::string id, std::string filePath, std::string type) :
	CScene(id, filePath, type)
{

	key_handler = new CPlayScenceKeyHandler(this);
	timeScale = DEFAULT_TIME_SCALE;
	

}


CMario* CPlayScene::GenerateMario(MarioType mType, Vector2 pos) {
	switch (mType)
	{
	case MarioType::SMALL:
		return new CSmallMario(pos.x, pos.y);
	case MarioType::TALL:
		return new CBigMario(pos.x, pos.y);
	case MarioType::TAIL:
		return new CTailMario(pos.x, pos.y);
	case MarioType::FIRE:
		return new CFireMario(pos.x, pos.y);
	default:
		return new CSmallMario(pos.x, pos.y);
	}
}


bool CPlayScene::LoadDataFromFile() {
	CScene::LoadDataFromFile();

	TiXmlDocument document(sceneFilePath.c_str());
	if (!document.LoadFile())
	{
		OutputDebugStringW(ToLPCWSTR(document.ErrorDesc()));
		return false;
	}

	TiXmlElement* root = document.RootElement();

	// Load Objects
	for (TiXmlElement* objs = root->FirstChildElement("objects"); objs != nullptr; objs = objs->NextSiblingElement("objects")) {
		// Mario
		for (TiXmlElement* mario = objs->FirstChildElement("mario"); mario != nullptr; mario = mario->NextSiblingElement("mario")) {
			if (player != NULL) break;
			int x = atoi(mario->Attribute("x"));
			int y = atoi(mario->Attribute("y"));
			int iType = 0;
			mario->QueryIntAttribute("type", &iType);
			playerLevel = iType;
			SwitchPlayer(GenerateMario((MarioType)playerLevel, Vector2(x, y)));
		}

		
	}

	std::string mapFilePath = root->Attribute("mapFilePath");

	sceneCamera.LoadMap(mapFilePath, &cameraLimitController, &staticObjects);
	if (cameraLimitController == NULL)
		sceneCamera.InitPositionController(player);
	else sceneCamera.InitPositionController(cameraLimitController);
	sceneCamera.InitMario(player);
	
	return true;
}

void CPlayScene::Load()
{
	LoadDataFromFile();
	BeginOpeningEffect();
	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::DownLevelMario() {

	if (playerLevel - 1 >= 0) playerLevel--;

	CMario* newMario = GenerateMario(
		(MarioType)(playerLevel),
		Vector2(player->x, player->y));
	newMario->BeginUntouchable();
	SwitchPlayer(newMario);

}

void CPlayScene::UpLevelMario() {

	if (playerLevel + 1 <= 3) playerLevel++;

	CMario* newMario = GenerateMario(
		(MarioType)(playerLevel),
		Vector2(player->x, player->y));
	newMario->BeginUntouchable();
	SwitchPlayer(newMario);

}

void CPlayScene::HandleMarioDie() {
}

void CPlayScene::UpdateEffects(DWORD dt) {
	for (size_t i = 0; i < effects.size(); i++)
	{
		if (!effects[i]->GetActiveState()) {
			effects.erase(effects.begin() + i);
			i--;
		}
		else
			effects[i]->Update(dt);
	}

}

void CPlayScene::HandleSceneTime(DWORD& dt) {
	if (((CMario*)(player))->GetAction() == MarioAction::EXPLODE ||
		((CMario*)(player))->GetAction() == MarioAction::UPGRADE_LV) {
		if (((CMario*)(player))->IsReadyToChangeAction()) {
			timeScale = DEFAULT_TIME_SCALE;
			if (((CMario*)(player))->GetAction() == MarioAction::EXPLODE)
				DownLevelMario();
			else
				UpLevelMario();
		}
		else
		{
			UpdateIfInCameraOrDisable(&enemyBullets, dt, &enemyBullets);
			timeScale = 0;
		}
	}
	else {
		timeScale = DEFAULT_TIME_SCALE;
	}

	dt = dt * timeScale;
}

void CPlayScene::SwitchToSelectionScene() {
	if (closingOpeningEffect.isActive == false) {
		BeginClosingEffect([]() {
			CGame::GetInstance()->SwitchScene("world1-selection");
		});
	}
		

}

void CPlayScene::Update(DWORD dt)
{
	
	ProcessBlackPortion(dt);
	if (player == NULL || isPausing) return;

	if (((CMario*)(player))->GetAction() == MarioAction::DIE) {
		
		player->Update(dt, &mainObjects);
		return;
	}

	HandleSceneTime(dt);

	vector<LPGAMEOBJECT> coObjects;
	vector<LPGAMEOBJECT> mapObjects;
	vector<LPGAMEOBJECT> dynamicObjects;
	vector<LPGAMEOBJECT> dynamicObjectsBehindMap;
	vector<LPGAMEOBJECT> highPriorityObjects;
	vector<LPGAMEOBJECT> tempObjects;
	

	vector<LPGAMEOBJECT> objsFromGrid = sceneCamera.GetMap()->GetGrid()->GetObjectsInGrid();
	GetObjsBaseOnPriority(&objsFromGrid, &mapObjects, EPriorityFlag::MAP_OBJECT);
	PushBackToCalculateCollision(&coObjects, &staticObjects);
	PushBackToCalculateCollision(&coObjects, &mapObjects);
	
	GetObjsBaseOnPriority(&objsFromGrid, &dynamicObjects, EPriorityFlag::DYNAMIC_OBJECT);
	GetObjsBaseOnPriority(&objsFromGrid, &dynamicObjectsBehindMap, EPriorityFlag::DYNAMIC_OBJECT_BEHIND_MAP);
	
	UpdateIfInCamera(&dynamicObjects, dt, &coObjects);
	UpdateIfInCamera(&dynamicObjectsBehindMap, dt, &coObjects);

	PushBackToCalculateCollision(&coObjects, &dynamicObjects);
	PushBackToCalculateCollision(&coObjects, &dynamicObjectsBehindMap);


	UpdateIfInCameraOrDisable(&mainObjects, dt, &coObjects);
	PushBackToCalculateCollision(&coObjects, &mainObjects);

	GetObjsBaseOnPriority(&objsFromGrid, &highPriorityObjects, EPriorityFlag::HIGH_PRIORITY_OBJECT);
	UpdateTempObjsInCamera(&highPriorityObjects, dt, &coObjects);

	GetObjsBaseOnPriority(&objsFromGrid, &tempObjects, EPriorityFlag::TEMP_OBJECT);
	UpdateTempObjsInCamera(&tempObjects, dt, &mainObjects);

	
	UpdateIfInCameraOrDisable(&enemyBullets, dt, &mainObjects);

	UpdateEffects(dt);

	if (cameraLimitController != NULL)
	cameraLimitController->Update(dt, nullptr);

	sceneCamera.GetMap()->GetGrid()->Update();
	sceneCamera.Update(dt); // Update Map in Camera

}

void CPlayScene::Render()
{
	vector<LPGAMEOBJECT> mapObjects;
	vector<LPGAMEOBJECT> dynamicObjects;
	vector<LPGAMEOBJECT> dynamicObjectsBehindMap;
	vector<LPGAMEOBJECT> highPriorityObjects;
	vector<LPGAMEOBJECT> tempObjects;
	vector<LPGAMEOBJECT> objsFromGrid = sceneCamera.GetMap()->GetGrid()->GetObjectsInGrid();
	
	GetObjsBaseOnPriority(&objsFromGrid, &dynamicObjects, EPriorityFlag::MAP_OBJECT);
	GetObjsBaseOnPriority(&objsFromGrid, &dynamicObjects, EPriorityFlag::DYNAMIC_OBJECT);
	GetObjsBaseOnPriority(&objsFromGrid, &dynamicObjectsBehindMap, EPriorityFlag::DYNAMIC_OBJECT_BEHIND_MAP);
	GetObjsBaseOnPriority(&objsFromGrid, &highPriorityObjects, EPriorityFlag::HIGH_PRIORITY_OBJECT);
	GetObjsBaseOnPriority(&objsFromGrid, &tempObjects, EPriorityFlag::TEMP_OBJECT);

	Vector2 camSize = sceneCamera.GetCamSize();

	if (((CMario*)(player))->GetAction() == MarioAction::GETTING_INTO_THE_PIPE)
		RenderIfEnableAndInCamera(&mainObjects);
	RenderIfEnableAndInCamera(&dynamicObjectsBehindMap);

	sceneCamera.Render();
	
	RenderIfEnableAndInCamera(&staticObjects);
	RenderIfEnableAndInCamera(&mapObjects);
	RenderIfEnableAndInCamera(&dynamicObjects);

	if (((CMario*)(player))->GetAction() != MarioAction::GETTING_INTO_THE_PIPE)
		RenderIfEnableAndInCamera(&mainObjects);

	RenderIfEnableAndInCamera(&tempObjects);
	RenderIfEnableAndInCamera(&highPriorityObjects);

	RenderIfEnableAndInCamera(&enemyBullets);



	for (size_t i = 0; i < effects.size(); i++)
	{
		Vector2 finalPos = sceneCamera.ConvertPosition(effects[i]->GetCurrentPosition());
		effects[i]->Render(finalPos);
	}
	sceneCamera.RenderDetailBoard();
	RenderBlackEffect();

	if (isPausing) sceneCamera.RenderPausing();
}


void CPlayScene::PrepareChangeCameraArea(Vector2 playerPos, Vector2 LeftTopLimit, Vector2 RightBottomLimit) {
	changeCamData.playerPos = playerPos;
	changeCamData.LeftTopLimit = LeftTopLimit;
	changeCamData.RightBottomLimit = RightBottomLimit;
}


void CPlayScene::ChangeCameraArea() {
	player->SetPosition(changeCamData.playerPos);
	sceneCamera.InitPositionController(player);
	cameraLimitController = NULL;
	sceneCamera.ChangeCamArea(changeCamData.LeftTopLimit, changeCamData.RightBottomLimit);
}



/*
	Unload current scene
*/

void CPlayScene::TogglePausingMode() {
	isPausing = !isPausing;
}


void CPlayScene::Unload()
{
	CleanObjList(staticObjects);
	CleanObjList(mainObjects);
	CleanObjList(enemyBullets);
	SetPlayer(NULL);

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}
MarioType CPlayScene::GetMarioType() {
	
	CMario* currentPlayer = (CMario*)(((CPlayScene*)this->GetPlayer()));
		
		return currentPlayer->GetType();
	
		
	
};

void CPlayScene::ChangeGoldenBricksInCameraToShowCoinState() {
	vector<LPGAMEOBJECT> mapObjects;
	vector<LPGAMEOBJECT> objsFromGrid = sceneCamera.GetMap()->GetGrid()->GetObjectsInGrid();
	GetObjsBaseOnPriority(&objsFromGrid, &mapObjects, EPriorityFlag::MAP_OBJECT);

	for (int i = 0; i < mapObjects.size(); i++) {
		if (sceneCamera.IsInCamera(mapObjects[i]->GetPosition())){
			if (dynamic_cast<CGoldenBrick*>(mapObjects[i])) {
				mapObjects[i]->RemovePriority(EPriorityFlag::MAP_OBJECT);
				mapObjects[i]->AddPriority(EPriorityFlag::DYNAMIC_OBJECT);
				if (((CGoldenBrick*)(mapObjects[i]))->HaveHiddenItem() == false)
				((CGoldenBrick*)(mapObjects[i]))->ChangeState(EBlockState::SHOW_HIDDEN_COIN, 5000);
			}
		}
	}
}

void CPlayScene::BeginVibratingCamera(DWORD totalTime) {
	sceneCamera.BeginVibrating(totalTime);
}

void CPlayScene::SwitchPlayer(LPGAMEOBJECT newPlayer) {
	if (!newPlayer) return;

	if (player != NULL) {
		float oldLeft, oldTop, oldRight, oldBottom;
		player->GetBoundingBox(oldLeft, oldTop, oldRight, oldBottom);
		float newLeft, newTop, newRight, newBottom;
		newPlayer->GetBoundingBox(newLeft, newTop, newRight, newBottom);
		newPlayer->y -= (newBottom - oldBottom);
		newPlayer->x -= (newRight - oldRight);
	}


	// Delete pointer of Old Mario in List Objects

	while (mainObjects.size() > 0) {
		delete mainObjects.at(0);
		mainObjects.erase(mainObjects.begin());
	}

	player = newPlayer;
	mainObjects.push_back(newPlayer);
	// Add Fire Bullet
	if (dynamic_cast<CFireMario*>(newPlayer)) {
		mainObjects.push_back(((CFireMario*)newPlayer)->GetBullet(0));
		mainObjects.push_back(((CFireMario*)newPlayer)->GetBullet(1));
	}
	sceneCamera.InitMario(newPlayer);
	if (cameraLimitController == NULL)
		sceneCamera.InitPositionController(newPlayer);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	if (((CPlayScene*)scence)->GetPausingStatus() && KeyCode != DIK_ESCAPE) return;
	CMario* currentPlayer = (CMario*)(((CPlayScene*)scence)->GetPlayer());
	CGame* gameInstance = CGame::GetInstance();

	currentPlayer->ProcessKeyboard(gameInstance->GenerateKeyboardEvent(KeyCode));
	switch (KeyCode)
	{
	case DIK_0:
		currentPlayer->Reset();
		break;
	case DIK_1: {
		((CPlayScene*)scence)->SwitchPlayer(
			new CSmallMario(currentPlayer->x, currentPlayer->y)
		);
		((CPlayScene*)scence)->SetPlayerLevel(0);
	}	break;

	case DIK_2: {
		((CPlayScene*)scence)->SwitchPlayer(
			new CBigMario(currentPlayer->x, currentPlayer->y)
		);
		((CPlayScene*)scence)->SetPlayerLevel(1);
	}	break;

	case DIK_3: {
		((CPlayScene*)scence)->SwitchPlayer(
			new CTailMario(currentPlayer->x, currentPlayer->y)
		);
		((CPlayScene*)scence)->SetPlayerLevel(2);
	}	break;

	case DIK_4: {
		((CPlayScene*)scence)->SwitchPlayer(
			new CFireMario(currentPlayer->x, currentPlayer->y)
		);
		((CPlayScene*)scence)->SetPlayerLevel(3);
	}	break;

	case DIK_ESCAPE: {
		((CPlayScene*)scence)->TogglePausingMode(); break;
	}	
				   //Dich chuyen tam thoi
	case DIK_5: {
		((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->PrepareChangeCameraArea(Vector2(219.0f, 1104.56f), Vector2(0.0f, 0.0f), Vector2(8448.0f, 1448.0f));
		((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->ChangeCameraArea();
		break;
	}
	case DIK_6: {
		((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->PrepareChangeCameraArea(Vector2(1575.0f, 900.0f), Vector2(0.0f, 0.0f), Vector2(8448.0f, 1448.0f));
		((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->ChangeCameraArea();
		break;
	}
	case DIK_8: {
		((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->PrepareChangeCameraArea(Vector2(5949.00f, 600.0f), Vector2(0.0f, 0.0f), Vector2(8448.0f, 1448.0f));
		((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->ChangeCameraArea();
		break;
	}
	case DIK_9: {
		((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->PrepareChangeCameraArea(Vector2(6751.00f, 351.00f), Vector2(0.0f, 0.0f), Vector2(8448.0f, 1448.0f));
		((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->ChangeCameraArea();
		break;
	}
	case DIK_7: {
		((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->PrepareChangeCameraArea(Vector2(4750.0f, 450.0f), Vector2(0.0f, 0.0f), Vector2(8448.0f, 1448.0f));
		((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->ChangeCameraArea();
		break;
	}
	}

}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	if (((CPlayScene*)scence)->GetPausingStatus()) return;
	CMario* currentPlayer = (CMario*)(((CPlayScene*)scence)->GetPlayer());
	CGame* gameInstance = CGame::GetInstance();
	currentPlayer->ProcessKeyboard(gameInstance->GenerateKeyboardEvent(KeyCode, false, true));
}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	if (((CPlayScene*)scence)->GetPausingStatus()) return;
	CGame* gameInstance = CGame::GetInstance();
	CMario* currentPlayer = (CMario*)(((CPlayScene*)scence)->GetPlayer());
	if (currentPlayer == NULL)return;
	std::vector<int> UnOrderProcessKey = { DIK_A, DIK_S };
	std::vector<int> OrderProcessKey = { DIK_LEFT,DIK_RIGHT, DIK_DOWN };


	for (int i = 0; i < UnOrderProcessKey.size(); i++) {
		if (gameInstance->IsKeyDown(UnOrderProcessKey[i])) {
			currentPlayer->ProcessKeyboard(gameInstance->GenerateKeyboardEvent(UnOrderProcessKey[i], true));
		}
	}

	for (int i = 0; i < OrderProcessKey.size(); i++) {
		if (gameInstance->IsKeyDown(OrderProcessKey[i])) {
			currentPlayer->ProcessKeyboard(gameInstance->GenerateKeyboardEvent(OrderProcessKey[i], true));
			return;
		}

	}
	currentPlayer->ProcessKeyboard(gameInstance->GenerateKeyboardEvent(-1, true));



}

