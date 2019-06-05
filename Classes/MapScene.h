#pragma once

#include "cocos2d.h"
#include "LevelData.h"

USING_NS_CC;

class MapScene : public Scene {
public:
	bool init() override;
	CREATE_FUNC(MapScene);

private:
	void _loadLevels();

	void _displayLevels();

	std::vector<LevelData> _levels;
};