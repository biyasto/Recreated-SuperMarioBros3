#include "NoteBlock.h"
#include "GoldenBrick.h"
#include "PlayScene.h"
#include "BoundingCoinEffect.h"
#include "TailAttackBox.h"
#include "AddingPointEffect.h"
#include "Mario.h"
#include "PlayScene.h"
#include "Leaf.h"


#define TIME_BOUNDING			3

CNoteBlock::CNoteBlock(Vector2 initPos, string hiddenItem) : CQuestionBlock(initPos) {
	virtualCenter = Vector2(0, 0);
	deltaRender = Vector2(0, 0);
	BoundCount = TIME_BOUNDING;
	if (hiddenItem == "") Secret = false;
	else Secret = true;
}

void CNoteBlock::Render(Vector2 finalPos)
{
		CAnimations::GetInstance()->Get(GetAnimationIdFromState())->Render(finalPos);
}


void CNoteBlock::BeingCollidedTop(LPGAMEOBJECT obj) {
	if (dynamic_cast<CMario*>(obj)) {
		if (BoundCount > 0) {
			((CMario*)(obj))->BeingBouncedAfterJumpInNoteBlock();
			if (Secret)BoundCount--;
		}
		else
		{
			BoundCount = TIME_BOUNDING;
			((CMario*)(obj))->SecretBeingBouncedAfterJumpInNoteBlock();
		}
	}
}

void CNoteBlock::BeingCollidedBottom(LPGAMEOBJECT obj) {
}

void CNoteBlock::BeingCollided(LPGAMEOBJECT obj) {

	}


void CNoteBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
}

Vector2 CNoteBlock::GetBoundingBoxSizeFromState() {
			return Vector2(48, 48);
}

void CNoteBlock::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	Vector2 size = GetBoundingBoxSizeFromState();

	left = x - size.x / 2;
	top = y - size.y / 2;
	right = x + size.x / 2;
	bottom = y + size.y / 2;

}

std::string CNoteBlock::GetAnimationIdFromState() {
		return "ani-music-note-block";
}

void CNoteBlock::OnHadCollided(LPGAMEOBJECT obj) {
	BeingCollided(obj);
};



