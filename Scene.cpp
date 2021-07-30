#include "Scene.h"
#include "Mario.h"
#include "SpriteManager.h"
#include "Const.h"
CScene::CScene(std::string id, std::string filePath, std::string type)
{
	this->id = id;
	this->sceneFilePath = filePath;
	this->type = type;
	sceneCamera = CCamera();
}


int CScene::GetLastCard() {
	if (cards.size() == 0) return -1;
	return cards[cards.size() - 1];
}

void CScene::GetObjsBaseOnPriority(vector<LPGAMEOBJECT>* fromList, vector<LPGAMEOBJECT>* toList, EPriorityFlag targetFlag) {
	for (int i = 0; i < fromList->size(); i++) {
		std::vector<EPriorityFlag> priorityFlag = fromList->at(i)->GetPriorityFlag();
		for (int j = 0; j < priorityFlag.size(); j++) {
			if (priorityFlag[j] == targetFlag) {
				toList->push_back(fromList->at(i));
			}
		}
	}
}

void CScene::UpdateTempObjsInCamera(vector<LPGAMEOBJECT>* objList, DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	for (size_t i = 0; i < objList->size(); i++)
	{
		if (!objList->at(i)->isTemp) {
			objList->at(i)->RemovePriority(EPriorityFlag::TEMP_OBJECT);
		}
		else if (sceneCamera.IsInCamera(Vector2(objList->at(i)->x, objList->at(i)->y), SCREEN_WIDTH / 2))
			objList->at(i)->Update(dt, coObjects);
	}
}


void CScene::UpdateIfInCameraOrDisable(vector<LPGAMEOBJECT>* objList, DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	
	for (size_t i = 0; i < objList->size(); i++)
	{
		if (!(dynamic_cast<CMario*>(objList->at(i))) && !sceneCamera.IsInCamera(Vector2(objList->at(i)->x, objList->at(i)->y), SCREEN_WIDTH / 2))
			objList->at(i)->isDisable = true;
		else if(!objList->at(i)->isDisable)
			objList->at(i)->Update(dt, coObjects);
	}
}

void CScene::UpdateIfInCamera(vector<LPGAMEOBJECT>* objList, DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	for (size_t i = 0; i < objList->size(); i++)
	{
		if (sceneCamera.IsInCamera(Vector2(objList->at(i)->x, objList->at(i)->y), SCREEN_WIDTH / 2) && !objList->at(i)->isDisable && !objList->at(i)->isTemp)
			objList->at(i)->Update(dt, coObjects);
	}
}

void CScene::PushBackToCalculateCollision(vector<LPGAMEOBJECT>* storageList, vector<LPGAMEOBJECT>* inputList) {
	for (size_t i = 0; i < inputList->size(); i++)
	{
		storageList->push_back(inputList->at(i));
	}
}

void CScene::ProcessBlackPortion(DWORD dt) {
	if (!closingOpeningEffect.isActive) return;

	if (closingOpeningEffect.currentBlackPortion == 0 - closingOpeningEffect.depreciation && closingOpeningEffect.isOpening
		|| closingOpeningEffect.currentBlackPortion == closingOpeningEffect.totalBlackPortion + closingOpeningEffect.depreciation && !closingOpeningEffect.isOpening)
	{
		closingOpeningEffect.isActive = false;
		if (closingOpeningEffect.callback != nullptr) closingOpeningEffect.callback();
	}

	if (closingOpeningEffect.isOpening) {
		if (closingOpeningEffect.currentBlackPortion - dt > 0 - closingOpeningEffect.depreciation)
			closingOpeningEffect.currentBlackPortion -= dt;
		else {
			closingOpeningEffect.currentBlackPortion = 0 - closingOpeningEffect.depreciation;
		}
	}
	else {
		if (closingOpeningEffect.currentBlackPortion + dt < closingOpeningEffect.totalBlackPortion + closingOpeningEffect.depreciation)
			closingOpeningEffect.currentBlackPortion += dt;
		else {
			closingOpeningEffect.currentBlackPortion = closingOpeningEffect.totalBlackPortion + closingOpeningEffect.depreciation;
		}
	}
}
void CScene::BeginOpeningEffect(CallbackTypeScene callback) {
	closingOpeningEffect.isActive = true;
	closingOpeningEffect.currentBlackPortion = closingOpeningEffect.totalBlackPortion;
	closingOpeningEffect.isOpening = true;
	closingOpeningEffect.callback = callback;
}

void CScene::BeginClosingEffect(CallbackTypeScene callback) {
	closingOpeningEffect.isActive = true;
	closingOpeningEffect.currentBlackPortion = 0;
	closingOpeningEffect.isOpening = false;
	closingOpeningEffect.callback = callback;
}

void CScene::RenderBlackEffect() {
	if (!closingOpeningEffect.isActive) return;
	LPDIRECT3DTEXTURE9 black = CTextures::GetInstance()->Get("black");
	Vector2 camSize = sceneCamera.GetCamSize();
	RECT rect; rect.left = 0; rect.top = 0; rect.right = camSize.x; rect.bottom = camSize.y;
	float positivePortion = (closingOpeningEffect.totalBlackPortion - closingOpeningEffect.currentBlackPortion) / closingOpeningEffect.totalBlackPortion;
	float negativePortion = (closingOpeningEffect.currentBlackPortion - closingOpeningEffect.totalBlackPortion) / closingOpeningEffect.totalBlackPortion;
	// Center To Left
	CGame::GetInstance()->DrawWithScaling(Vector2((camSize.x * negativePortion) / 2, camSize.y / 2), Vector2(0, 0), black, rect, 255);
	// Center To Right
	CGame::GetInstance()->DrawWithScaling(Vector2(camSize.x + (camSize.x * positivePortion) / 2, camSize.y / 2), Vector2(0, 0), black, rect, 255);
	
	// Center To Top
	CGame::GetInstance()->DrawWithScaling(Vector2(camSize.x / 2, (camSize.y * negativePortion) / 2), Vector2(0, 0), black, rect, 255);
	// Center To Bottom
	CGame::GetInstance()->DrawWithScaling(Vector2(camSize.x / 2, camSize.x + (camSize.y * positivePortion) / 2), Vector2(0, 0), black, rect, 255);



}

void CScene::RenderIfEnableAndInCamera(vector<LPGAMEOBJECT>* objList) {
	for (int i = 0; i < objList->size(); i++) {
		LPGAMEOBJECT obj = objList->at(i);
		if (!obj->isDisable) {
			if (sceneCamera.IsInCamera(obj->GetPosition())) {
				Vector2 finalPos = sceneCamera.ConvertPosition(obj->GetPosition());
				obj->Render(finalPos);
			}
		}
	}
		
}


bool CScene::LoadDataFromFile() {
	TiXmlDocument document(sceneFilePath.c_str());
	if (!document.LoadFile())
	{
		OutputDebugStringW(ToLPCWSTR(document.ErrorDesc()));
		return false;
	}

	TiXmlElement* root = document.RootElement();



	// Load Texture
	for (TiXmlElement* textures = root->FirstChildElement("textures"); textures != nullptr; textures = textures->NextSiblingElement("textures")) {
		for (TiXmlElement* texture = textures->FirstChildElement("texture"); texture != nullptr; texture = texture->NextSiblingElement("texture")) {
			std::string id = texture->Attribute("id");
			std::string filePath = texture->Attribute("filePath");
			auto transColor = texture->Attribute("transColor");
			if (transColor != NULL) {
				auto rgb = split(transColor, ",");
				CTextures::GetInstance()->Add(id, ToLPCWSTR(filePath), D3DCOLOR_XRGB(atoi(rgb[0].c_str()), atoi(rgb[1].c_str()), atoi(rgb[2].c_str())));
			}
			else {
				CTextures::GetInstance()->Add(id, ToLPCWSTR(filePath), D3DXCOLOR());
			}

		}
	}

	// Load Sprite
	for (TiXmlElement* sprites = root->FirstChildElement("sprites"); sprites != nullptr; sprites = sprites->NextSiblingElement("sprites")) {
		for (TiXmlElement* sprite = sprites->FirstChildElement("sprite"); sprite != nullptr; sprite = sprite->NextSiblingElement("sprite")) {
			//std::string texId = sprite->Attribute("texId");
			std::string filePath = sprite->Attribute("filePath");
			CSprites::GetInstance()->LoadSpriteFromFile(filePath);
		}
	}

	// Load Animations
	for (TiXmlElement* animations = root->FirstChildElement("animations"); animations != nullptr; animations = animations->NextSiblingElement("animations")) {
		for (TiXmlElement* animation = animations->FirstChildElement("animation"); animation != nullptr; animation = animation->NextSiblingElement("animation")) {
			std::string idSet = animation->Attribute("idSet");
			std::string filePath = animation->Attribute("filePath");
			CAnimations::GetInstance()->LoadAnimationsFromFile(filePath, idSet);

		}
	}

	// Load Limit Area
	for (TiXmlElement* limitArea = root->FirstChildElement("limitArea"); limitArea != nullptr; limitArea = limitArea->NextSiblingElement("limitArea")) {
		auto left = limitArea->Attribute("left");
		auto top = limitArea->Attribute("top");
		auto right = limitArea->Attribute("right");
		auto bottom = limitArea->Attribute("bottom");
		if (left != NULL && top != NULL && right != NULL && bottom != NULL) {
			sceneCamera.ChangeCamArea(Vector2(atoi(left), atoi(top)), Vector2(atoi(right), atoi(bottom)));
		}
	}

}


void CScene::CleanObjList(vector<LPGAMEOBJECT>& objList) {
	for (int i = 0; i < objList.size(); i++)
		if(objList[i] != nullptr)
			delete objList[i];

	objList.clear();
}