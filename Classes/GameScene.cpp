#include "GameScene.h"
#include "MapScene.h"
#include "json/document.h"

using namespace rapidjson;

void GameScene::AcceptMessage(std::string message, std::string data)
{
	if (message == "Match") {
		_points += std::atoi(data.c_str());
		_gl->SetCurrentValue(_points);
	}
	if (message == "Swap") {
		--_moves;
		_mll->SetValue(_moves);
	}
	if (message == "Stability") {
		if (_points >= _goal) {
			_setState(GameSceneState::Win);
		}
		else if (_moves <= 0) {
			_setState(GameSceneState::Lose);
		}
	}
}

bool GameScene::init(std::string src)
{
	if (!Scene::init()) {
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 center(visibleSize.width / 2, visibleSize.height / 2);

	_gameField = GameField::create();
	_gameField->setScale(0.4f);
	_gameField->setPosition(center);
	this->addChild(_gameField);

	_gameField->Subscribe(this);

	_loadLvl(src);

	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		[](Ref* pSender) {
		Director::getInstance()->replaceScene(MapScene::create());
	});

	float x = visibleSize.width - closeItem->getContentSize().width / 2;
	float y = closeItem->getContentSize().height / 2;
	closeItem->setPosition(Vec2(x, y));

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	_mll = MovesLeftLabel::create();
	_mll->setAnchorPoint(Vec2(1.f, 1.f));
	_mll->setPosition(Vec2(visibleSize.width, visibleSize.height));
	_mll->setSystemFontSize(21);
	_mll->SetValue(_moves);
	this->addChild(_mll);

	_gl = GoalLabel::create();
	_gl->setAnchorPoint(Vec2(1.f, 1.f));
	_gl->setPosition(Vec2(visibleSize.width, visibleSize.height - 50));
	_gl->setSystemFontSize(21);
	_gl->SetCurrentValue(_points);
	_gl->SetRequireValue(_goal);
	this->addChild(_gl);

	_winLabel = Label::create("You Win", "SnowDream.TTF", 32);
	_winLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
	_winLabel->setPosition(center);
	_winLabel->setZOrder(5);
	_winLabel->setVisible(false);
	this->addChild(_winLabel);

	_loseLabel = Label::create("You Lose", "SnowDream.TTF", 32);
	_loseLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
	_loseLabel->setPosition(center);
	_loseLabel->setZOrder(5);
	_loseLabel->setVisible(false);
	this->addChild(_loseLabel);

	return true;
}

void GameScene::_setState(GameSceneState newState)
{
	_state = newState;
	if (_state == GameSceneState::Play) {
		_gameField->EnableInput();
	}
	else {
		_gameField->DisableInput();
		if (_state == GameSceneState::Win) {
			_winLabel->setVisible(true);
		}
		else {
			_loseLabel->setVisible(true);
		}
	}
}

void GameScene::_loadLvl(std::string src)
{
	std::string jsonData = FileUtils::getInstance()->getStringFromFile(src);

	Document document;
	document.Parse(jsonData.c_str());

	if (document.IsObject()) {
		log("it's ok");
	}
	else {
		log("it's not ok");
	}

	int height = document["height"].GetInt();
	int width = document["width"].GetInt();

	auto arr = document["map"].GetArray();

	std::vector<std::vector<int>> map(width, std::vector<int>(height, 1));

	for (int ix = 0; ix < width; ix++) {
		for (int iy = 0; iy < height; iy++) {
			map[ix][iy] = arr[ix].GetArray()[iy].GetInt();
		}
	}

	_gameField->SetMap(width, height, map);

	_goal = document["goal"].GetInt();
	_moves = document["moves"].GetInt();

}
