#include "MapScene.h"
#include "LevelPreview.h"
#include "DataManager.h"
#include "GameScene.h"

void MapScene::AcceptMessage(std::string message, std::string data)
{
	if (message == "Win") {
		int index = std::atoi(data.c_str());
		_levels[index].isFinish = true;
		_updateLevels();
	}
}

bool MapScene::init()
{
	if (!Scene::init()) {
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 center(visibleSize.width / 2, visibleSize.height / 2);

	auto back = Sprite::create("textures/back/back_1.png");
	back->setAnchorPoint(Vec2::ZERO);
	this->addChild(back);

	_levelsOrigin = Node::create();
	this->addChild(_levelsOrigin);
	
	_loadLevels();
	_displayLevels();

	auto closeItem = MenuItemImage::create(
		"textures/ui_assets/button_close01.png",
		"textures/ui_assets/button_close02.png",
		[&](Ref* pSender) {
		_saveLevels();
		Director::getInstance()->end();
	});

	float x = visibleSize.width - closeItem->getContentSize().width / 2;
	float y = closeItem->getContentSize().height / 2;
	closeItem->setPosition(Vec2(x, y));

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	return true;
}

void MapScene::_loadLevels()
{
	_levels = DataManager::GetInstance()->GetLevelData(_levelsSrc);
	
}

void MapScene::_saveLevels()
{
	DataManager::GetInstance()->SaveLevelData(_levels, _levelsSrc);
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
		_levelsOrigin->addChild(preview);

		preview->SetCallback([=](Ref* pSender) {
			auto gs = GameScene::create(_levels[i].source);
			gs->Subscribe(this);
			Director::getInstance()->pushScene(gs);
		});
	}
}

void MapScene::_updateLevels()
{
	_levelsOrigin->removeAllChildren();
	_displayLevels();
}
