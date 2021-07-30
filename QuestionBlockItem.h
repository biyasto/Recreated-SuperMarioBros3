#pragma once
#include "QuestionBlock.h"

class CQuestionBlockItem : public CQuestionBlock
{
protected:

public:
	CQuestionBlockItem(Vector2);
	virtual void OpenBox() override;
};

