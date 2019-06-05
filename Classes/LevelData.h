#pragma once

#include "cocos2d.h"

struct LevelData {

	LevelData(int number, std::string source, bool isFinish) {
		this->number = number;
		this->source = source;
		this->isFinish = isFinish;
	}

	int number = 0;
	std::string source = "";
	bool isFinish = false;
};