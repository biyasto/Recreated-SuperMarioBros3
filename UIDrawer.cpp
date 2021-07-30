#include "UIDrawer.h"
#include <Windows.h>
#include "Utils.h"
#include "SpriteManager.h"

CUIDrawer* CUIDrawer::__instance = NULL;

CUIDrawer::CUIDrawer()
{

}

CUIDrawer* CUIDrawer::GetInstance()
{
	if (__instance == NULL) __instance = new CUIDrawer();
	return __instance;
}

void CUIDrawer::Draw(std::string inp, Vector2 finalPos){
	for (int i = 0; i < inp.length(); i++) {
		std::string sprId = string(1, inp[i]) == " " ? "spr-font-space" : "spr-font-" + string(1, inp[i]);
		CSprites::GetInstance()->Get(sprId)->DrawWithScaling(finalPos + Vector2(i*26,0));
	}
}

void CUIDrawer::DrawFixedLengthNumber(std::string inp, Vector2 finalPos, char defaultChar, int numLength) {
	while (inp.length() < numLength)	inp = string(1, defaultChar) + inp;
	for (int i = 0; i < inp.length(); i++) {
		CSprites::GetInstance()->Get("spr-font-" + string(1, inp[i]))->DrawWithScaling(finalPos + Vector2(i * 26, 0));
	}
}