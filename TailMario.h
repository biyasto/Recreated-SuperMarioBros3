#pragma once
#include "Mario.h"
#include "TailAttackBox.h"

class CTailMario : public CMario
{
private:
	
	CTailAttackBox attackBoundingBox;

public:
	CTailMario(float x = 0.0f, float y = 0.0f);
	void Render(Vector2 finalPos);
	void Update(DWORD, vector<LPGAMEOBJECT>* = NULL) override;
	

	void ProcessKeyboard(SKeyboardEvent kEvent);
};

