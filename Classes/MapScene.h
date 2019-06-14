#pragma once

#include "cocos2d.h"
#include "LevelData.h"
#include "MessageListener.h"

USING_NS_CC;

class MapScene : public Scene, public MessageListener {
public:
	CREATE_FUNC(MapScene);

	void AcceptMessage(std::string message, std::string data) override;

private:
	bool init() override;

	void _loadLevels();
	void _saveLevels();

	void _displayLevels();

	void _updateLevels();

	Node* _levelsOrigin;

	std::vector<LevelData> _levels;

	std::string _levelsSrc = "json/levels.json";
};