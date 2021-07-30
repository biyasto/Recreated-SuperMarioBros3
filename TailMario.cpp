#include "TailMario.h"
#include "Mario.h"
#define DISTANCE_OF_ATTACK_BOUNDINGBOX_FROM_CENTER		Vector2(0, 20)


CTailMario::CTailMario(float x, float y) : CMario(x, y) {
	type = MarioType::TAIL;
	attackBoundingBox = CTailAttackBox();
}

void CTailMario::Render(Vector2 finalPos)
{
	CMario::Render(finalPos);
	
}

void CTailMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (state.action == MarioAction::ATTACK) attackBoundingBox.SetIsOpening(true);
	else attackBoundingBox.SetIsOpening(false);

	CMario::Update(dt, coObjects);
	attackBoundingBox.UpdatePosition(GetPosition() + DISTANCE_OF_ATTACK_BOUNDINGBOX_FROM_CENTER);
	attackBoundingBox.Update(dt, coObjects);
}


void CTailMario::ProcessKeyboard(SKeyboardEvent kEvent)
{
	CMario::ProcessKeyboard(kEvent);

	switch (kEvent.key)
	{
		//Attack
	case DIK_A:
		if (!kEvent.isHold && !kEvent.isKeyUp) {
			if (ChangeAction(MarioAction::ATTACK, 430))
				attackBoundingBox.SetHasAttacked(false);
		}
		break;

	case DIK_S:
		{ //Fly and Float
		if (!kEvent.isHold && !kEvent.isKeyUp) ChangeAction(MarioAction::FLY, 200);
		if (!kEvent.isHold && !kEvent.isKeyUp) ChangeAction(MarioAction::FALL_SLIGHTLY, 200);
		break;
		}
	}

	
	}
	



