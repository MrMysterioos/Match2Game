#pragma once
#include "cocos2d.h"

struct BuildLevelData {
	int lvlId;
	int goal;
	int moves;
	int width;
	int height;
	std::vector<std::vector<int>> matrix;
};