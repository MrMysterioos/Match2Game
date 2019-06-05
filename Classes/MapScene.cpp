#include "MapScene.h"
#include "json/document.h"
#include "LevelPreview.h"

using namespace rapidjson;

bool MapScene::init()
{
	if (!Scene::init()) {
		return false;
	}
	
	_loadLevels();
	_displayLevels();

	return true;
}

void MapScene::_loadLevels()
{
	std::string jsonData = FileUtils::getInstance()->getStringFromFile("json/levels.json");

	Document document;
	document.Parse(jsonData.c_str());

	if (document.IsArray()) {
		log("ok");
		auto arr = document.GetArray();
		for (auto& el : arr) {
			_levels.push_back(LevelData(el["num"].GetInt(), el["src"].GetString(), el["fin"].GetBool()));
		}
	}
	else {
		log("not ok");
	}

}

void MapScene::_displayLevels()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Vec2 center = Vec2(visibleSize.width / 2, visibleSize.height / 2);

	int colums = 3;
	int spacingX = 300;
	int spacingY = 200;

	int rows = _levels.size() % colums ? _levels.size() / colums + 1 : _levels.size() / colums;
	Vec2 offset = -Vec2((float)(colums - 1) / 2 * spacingX, -(float)(rows - 1) / 2 * spacingY);

	for (int i = 0; i < _levels.size(); i++) {
		
		LevelPreview* preview = LevelPreview::create(_levels[i]);
		preview->setPosition(center + offset + Vec2((i % colums) * spacingX, -(i / colums) * spacingY));
		this->addChild(preview);
	}
}
