#pragma once
#include "Transform.h"
#include <unordered_map>
#include <string>



class CSelectionNode
{
	std::string name;
	Vector2 position;
	std::unordered_map<std::string, std::string> movableNode;

public:
	CSelectionNode(std::string, Vector2);
	void AddMovableNode(std::string, std::string);
	Vector2 GetPosition();
	std::string GetTargetNode(std::string);
	std::string GetName() { return name; }
};



