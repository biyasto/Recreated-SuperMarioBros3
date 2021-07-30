#include "QuestionBlockItem.h"
#include "Leaf.h"
#include "PlayScene.h"
#include "Mushroom.h"

CQuestionBlockItem::CQuestionBlockItem(Vector2 initPos) : CQuestionBlock(initPos) {

}

void CQuestionBlockItem::OpenBox() {
	LPGAMEOBJECT obj;
	if (((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->GetMarioType() != MarioType::SMALL)
	{		obj = new CLeaf(GetPosition() - Vector2(0, 48));
	obj->AddPriority(EPriorityFlag::TEMP_OBJECT);
	
	}
	//La hoac Hoa
	else
	{ //Nam
		obj = new Mushroom(GetPosition() - Vector2(0, 48), true);
		obj->isTemp = false;
		obj->AddPriority(EPriorityFlag::DYNAMIC_OBJECT);
	}
	((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->GetCamera()->GetMap()->GetGrid()->AddObjectToGrid(obj);
	

}