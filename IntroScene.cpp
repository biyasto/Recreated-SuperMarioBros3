#include "IntroScene.h"

#include "SelectionSceneMario.h"

#define MOVING_DISTANCE				3.5f

CIntroScene::CIntroScene(std::string id, std::string filePath, std::string type) :
	CScene(id, filePath, type)
{

	key_handler = new CIntroSceneKeyHandler(this);
	timeScale = DEFAULT_TIME_SCALE;

}

bool CIntroScene::GetCursor(vector<LPGAMEOBJECT>* listObjs, CIntroSceneObj** receiver)
{
	for (int i = 0; i < listObjs->size(); i++) {
		if (dynamic_cast<CIntroSceneObj*>(listObjs->at(i))) {
			if (((CIntroSceneObj*)(listObjs->at(i)))->GetName() == "OptionCursor") {
				*receiver = (CIntroSceneObj*)(listObjs->at(i));
				return true;
			}
		}
	}
	return false;
}
void CIntroScene::Load()
{
	LoadDataFromFile();
	if (!GetCursor(&objs, &cursor)) {
		DebugOut(L"[INFO] Cannot Get Cursor in Intro Scene %s\n", sceneFilePath);
	}
	//BeginOpeningEffect();
	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CIntroScene::Update(DWORD dt)
{

	ProcessBlackPortion(dt);
}

void CIntroScene::Render()
{
	sceneCamera.Render();
	RenderIfEnableAndInCamera(&objs);
	
	RenderBlackEffect();
}

void CIntroScene::Unload()
{
	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CIntroScene::ChangeOption(int delta)
{
	if (selectedOption + delta < 0 || selectedOption + delta > 2) return;
	selectedOption += delta;
	cursor->ChangeDeltaRender(Vector2(0, selectedOption * 50));
}


void CIntroSceneKeyHandler::OnKeyDown(int KeyCode)
{
	
	switch (KeyCode)
	{
	case DIK_UP: {
		((CIntroScene*)scence)->ChangeOption(-1);
		break;
	}
	case DIK_DOWN: {
		((CIntroScene*)scence)->ChangeOption(1);
		break;
	}
	case DIK_S:
		((CIntroScene*)scence)->BeginClosingEffect([]() {
			CGame::GetInstance()->SwitchScene("world1-selection");
		});

		break;
	}
}


bool CIntroScene::LoadDataFromFile() {
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
	sceneCamera.LoadMap(mapFilePath, &objs);
	sceneCamera.ChangeCamArea(Vector2(0, 0), Vector2(sceneCamera.GetMapSize().x, sceneCamera.GetMapSize().y));
	

	// Load Objects
	for (TiXmlElement* objs = root->FirstChildElement("objects"); objs != nullptr; objs = objs->NextSiblingElement("objects")) {
		// Mario
		
	}
	return true;
}