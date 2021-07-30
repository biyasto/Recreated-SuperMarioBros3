#include "Animation.h"
#include "SpriteManager.h"
#include "XmlReader/tinyxml.h"


CAnimation::CAnimation(int defaultTime) {
	if (defaultTime > 0) this->defaultTime = defaultTime;
	else this->defaultTime = 100;
	lastFrameTime = -1; 
	currentFrame = -1; 
}

void CAnimation::Add(std::string spriteId, DWORD time)
{
	int t = time;
	if (!(time > 0)) t = this->defaultTime;

	LPSPRITE sprite = CSprites::GetInstance()->Get(spriteId);

	if (sprite == NULL)
	{
		DebugOut(L"[ERROR] Sprite ID %d cannot be found!\n", spriteId);
	}

	LPANIMATION_FRAME frame = new CAnimationFrame(sprite, t);
	frames.push_back(frame);
}





bool CAnimations::LoadAnimationsFromFile(std::string filePath, std::string setId) {
	OutputDebugStringW(ToLPCWSTR(filePath.c_str()));
	TiXmlDocument document(filePath.c_str());
	if (!document.LoadFile())
	{
		OutputDebugStringW(ToLPCWSTR(document.ErrorDesc()));
		return false;
	}

	TiXmlElement* root = document.RootElement();
	TiXmlElement* info = root->FirstChildElement();

	string gameObjectID = info->Attribute("gameObjectId");
	string textureID = info->Attribute("textureId");

	OutputDebugStringW(ToLPCWSTR("Gameobject id: " + gameObjectID + '\n'));
	OutputDebugStringW(ToLPCWSTR("Texture id: " + textureID + '\n'));

	for (TiXmlElement* node = info->FirstChildElement(); node != nullptr; node = node->NextSiblingElement())
	{
		string aniId = node->Attribute("aniId");
		float frameTime;
		node->QueryFloatAttribute("frameTime", &frameTime);
		OutputDebugStringW(ToLPCWSTR(aniId + ':' + to_string(frameTime) + '\n'));
		if(!(frameTime > 0)) frameTime = 100;
		CAnimation* animation = new CAnimation(frameTime);

		// Sprite ref
		for (TiXmlElement* sprNode = node->FirstChildElement(); sprNode != nullptr; sprNode = sprNode->NextSiblingElement())
		{
			string spriteId = sprNode->Attribute("id");

			float detailFrameTime;
			sprNode->QueryFloatAttribute("frameTime", &detailFrameTime);
			if (!(detailFrameTime > 0)) detailFrameTime = frameTime;
			animation->Add(spriteId, detailFrameTime);

			//OutputDebugStringW(ToLPCWSTR("|--" + id + ':' + to_string(detailFrameTime) + '\n'));
		}

		CAnimations::GetInstance()->Add(aniId, animation);
		

	}


	return true;
}




void CAnimation::Render(Vector2 finalPos, Vector2 scale, int alpha)
{
	DWORD now = GetTickCount();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size()) currentFrame = 0;
		}
	}

	
	frames[currentFrame]->GetSprite()->DrawWithScaling(finalPos, scale, alpha);

	
}


CAnimations* CAnimations::__instance = NULL;

CAnimations* CAnimations::GetInstance()
{
	if (__instance == NULL) __instance = new CAnimations();
	return __instance;
}

void CAnimations::Add(std::string id, LPANIMATION ani)
{
	animations[id] = ani;
}

LPANIMATION CAnimations::Get(std::string id)
{
	LPANIMATION ani = animations[id];
	if (ani == NULL)
		DebugOut(L"[ERROR] Failed to find animation id: %d\n", id);
	return ani;
}

void CAnimations::Clear()
{
	for (auto x : animations)
	{
		LPANIMATION ani = x.second;
		delete ani;
	}

	animations.clear();
}