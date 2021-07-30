#include "SelectionScene.h"
#include "SelectionSceneMario.h"

#define MOVING_DISTANCE				3.5f

CSelectionScene::CSelectionScene(std::string id, std::string filePath, std::string type) :
	CScene(id, filePath, type)
{

	key_handler = new CSelectionSceneKeyHandler(this);
	timeScale = DEFAULT_TIME_SCALE;

}

void CSelectionScene::Load()
{
	LoadDataFromFile();
	BeginOpeningEffect();
	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CSelectionScene::Update(DWORD dt)
{
	
	if (isMoving) {
		int distanceX = player->x - standingNode->GetPosition().x;
		int distanceY = player->y - standingNode->GetPosition().y;
		if (distanceX == 0 && distanceY == 0) isMoving = false;
		if (abs(distanceX) < MOVING_DISTANCE) player->x -= distanceX;
		else player->x += MOVING_DISTANCE * (distanceX < 0 ? 1 : -1);
		if (abs(distanceY) < MOVING_DISTANCE) player->y -= distanceY;
		else player->y += MOVING_DISTANCE * (distanceY < 0 ? 1 : -1);
	}
	ProcessBlackPortion(dt);
}

void CSelectionScene::Render()
{
	sceneCamera.Render();
	for (int i = 0; i < selectionPortals.size(); i++)
		selectionPortals[i]->Render(sceneCamera.ConvertPosition(Vector2(selectionPortals[i]->x, selectionPortals[i]->y)));
	player->Render(sceneCamera.ConvertPosition(Vector2(player->x, player->y)));
	sceneCamera.RenderDetailBoard();
	RenderBlackEffect();
}

void CSelectionScene::Unload()
{
	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CSelectionScene::ProcessMovingToNewNode(int KeyCode) {
	if (isMoving) return;
	std::string strKeyCode;

	if		(KeyCode == DIK_LEFT)	strKeyCode = "left";
	else if (KeyCode == DIK_UP)		strKeyCode = "up";
	else if (KeyCode == DIK_RIGHT)	strKeyCode = "right";
	else if (KeyCode == DIK_DOWN)	strKeyCode = "down";

	std::string newNode = standingNode->GetTargetNode(strKeyCode);
	if (newNode != "") {
		isMoving = true;
		standingNode = selectionNodes[newNode];
		
	}
}

void CSelectionSceneKeyHandler::OnKeyDown(int KeyCode)
{
	if (KeyCode == DIK_LEFT || KeyCode == DIK_UP || KeyCode == DIK_RIGHT || KeyCode == DIK_DOWN) {
		((CSelectionScene*)scence)->ProcessMovingToNewNode(KeyCode);
		return;
	}
	switch (KeyCode)
	{
		
		case DIK_S:
		{
			std::string nodeName = ((CSelectionScene*)scence)->GetStandingNode()->GetName();
			std::string targetSceneId;
			//MAP1
			if (nodeName == "node-1") {
				((CSelectionScene*)scence)->BeginClosingEffect([]() {
					CGame::GetInstance()->SwitchScene("world1-1");
				});
			}
			//MAP3
			else if (nodeName == "node-3") {
				((CSelectionScene*)scence)->BeginClosingEffect([]() {
					CGame::GetInstance()->SwitchScene("world1-3");
					});
			}
			break;
		}
		
	}
}


bool CSelectionScene::LoadDataFromFile() {
	CScene::LoadDataFromFile();
	TiXmlDocument document(sceneFilePath.c_str());
	if (!document.LoadFile())
	{
		OutputDebugStringW(ToLPCWSTR(document.ErrorDesc()));
		return false;
	}

	TiXmlElement* root = document.RootElement();


	std::string mapFilePath = root->Attribute("mapFilePath");
	sceneCamera.InitPositionController(player);
	sceneCamera.LoadMap(mapFilePath, &selectionPortals, &selectionNodes);
	sceneCamera.ChangeCamArea(Vector2(0, 0), Vector2(sceneCamera.GetMapSize().x, sceneCamera.GetMapSize().y));
	if(player == NULL)
		standingNode = selectionNodes["node-0"];
	
	// Load Objects
	for (TiXmlElement* objs = root->FirstChildElement("objects"); objs != nullptr; objs = objs->NextSiblingElement("objects")) {
		// Mario
		for (TiXmlElement* mario = objs->FirstChildElement("mario"); mario != nullptr; mario = mario->NextSiblingElement("mario")) {
			if (player != NULL) break;
			Vector2 initPos = standingNode->GetPosition();
			player = new CSelectionSceneMario(Vector2(initPos.x, initPos.y));
		}
	}
	return true;
}