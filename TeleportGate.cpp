#include "TeleportGate.h"
#include "Mario.h"
#include "PlayScene.h"

CTeleportGate::CTeleportGate(float x, float y, float width, float height, Vector2 position, Vector2 limit1, Vector2 limit2) : CRectCollision(x, y, width, height) {
	this->teleport_position = position;
	this->LimitTopLeft = limit1;
	this->LimitBotRight = limit2;
}

void CTeleportGate::BeingCollided(LPGAMEOBJECT obj) {
	if (obj == NULL) return;
	
		if (dynamic_cast<CMario*>(obj)) {
			//((CMario*)obj)->Teleport(127,1829,0,0);

			if ((((CMario*)obj))->GetAction() != MarioAction::FLY) {
				((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->PrepareChangeCameraArea(teleport_position, LimitTopLeft, LimitBotRight);
				((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->ChangeCameraArea();
				((CMario*)obj)->vy = 0.2;
			}
		}
	}