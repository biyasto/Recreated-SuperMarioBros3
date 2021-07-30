#include "SelectionNode.h"
#include <dinput.h>

CSelectionNode::CSelectionNode(std::string nodeName, Vector2 pos) {
	this->name = nodeName;
	this->position = pos;
}

void CSelectionNode::AddMovableNode(std::string key, std::string nodeName) {
	movableNode[key] = nodeName;
}

Vector2 CSelectionNode::GetPosition()
{
	return position;
}

std::string CSelectionNode::GetTargetNode(std::string key)
{
	return movableNode[key];
}
