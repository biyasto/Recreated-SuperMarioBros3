#include "PipePortal.h"
#include "PlayScene.h"
#include "Mario.h"


CPipePortal::CPipePortal(std::string accessKeycode, Vector2 portalPosition, Vector2 portalSize, Vector2 portalTargetPos, Vector2 portalLimit1, Vector2 portalLimit2) {
	this->isTemp = true;
	this->accessKeycode = accessKeycode;
	this->x = portalPosition.x;
	this->y = portalPosition.y;
	this->portalSize = portalSize;
	this->targetPosition = portalTargetPos;

	this->targetCameraLeffTopLimit = portalLimit1;
	this->targetCameraRightBottomLimit = portalLimit2;
}
void CPipePortal::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	vx = vy = 0;
	CGameObject::Update(dt);
	UpdateWithCollision(coObjects);
}

void CPipePortal::OnHadCollided(LPGAMEOBJECT obj) {
	if (dynamic_cast<CMario*>(obj)) {
		short keyCondition = accessKeycode == "DOWN" ? DIK_DOWN : DIK_UP;
		short sign = keyCondition == DIK_DOWN ? 1 : -1;

		if (((CMario*)(obj))->isHoldingKey(keyCondition)) {
			((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->PrepareChangeCameraArea(targetPosition, targetCameraLeffTopLimit, targetCameraRightBottomLimit);
			
			Vector2 marioSize = ((CMario*)(obj))->GetBoundingBoxSize();
			((CMario*)(obj))->GetIntoThePipe(Vector2(0, marioSize.y * sign), []() {
				((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->ChangeCameraArea();
				
			});
			
		}
		return;
	}
}


void CPipePortal::Render(Vector2 finalPos) {
}


void CPipePortal::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x - portalSize.x / 2;
	top = y - portalSize.y / 2;
	right = x + portalSize.x / 2;
	bottom = y + portalSize.y / 2;
}