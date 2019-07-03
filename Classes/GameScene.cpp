#include "GameScene.h"
#include "MapScene.h"
#include "DataManager.h"

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
			Send("Win", std::to_string(_lvlId));
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

	auto back = Sprite::create("textures/back/back_1.png");
	back->setAnchorPoint(Vec2::ZERO);
	this->addChild(back);

	_gameField = GameField::create();
	_gameField->setScale(0.4f);
	_gameField->setPosition(center);
	this->addChild(_gameField);

	_gameField->Subscribe(this);

	_loadLvl(src);
	
	Size GFSize = _gameField->GetSize();
	Vec2 desireScale = Vec2((float)visibleSize.width / GFSize.width, (float)visibleSize.height / GFSize.height);
	if (desireScale.x < _gameField->getScale()) {
		_gameField->setScale((float)visibleSize.width / GFSize.width);
	}
	if (desireScale.y < _gameField->getScale()) {
		_gameField->setScale((float)visibleSize.height / GFSize.height);
	}

	auto closeItem = MenuItemImage::create(
		"textures/ui_assets/button_home01.png",
		"textures/ui_assets/button_home02.png",
		[](Ref* pSender) {
		Director::getInstance()->popScene();
	});

	float x = visibleSize.width - closeItem->getContentSize().width / 2;
	float y = closeItem->getContentSize().height / 2;
	closeItem->setPosition(Vec2(x, y));

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	_mll = MovesLeftLabel::create();
	_mll->setPosition(Vec2(0.f, visibleSize.height - 25));
	_mll->SetValue(_moves);
	this->addChild(_mll);

	_gl = GoalLabel::create();
	_gl->setPosition(Vec2(0.f, visibleSize.height - 100));
	_gl->SetCurrentValue(_points);
	_gl->SetRequireValue(_goal);
	this->addChild(_gl);

	_winLabel = Node::create();
	_winLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
	_winLabel->setPosition(center);
	_winLabel->setZOrder(5);
	_winLabel->setVisible(false);

	auto winText = Label::create("You Win", "fonts/HVD_Comic_Serif_Pro.otf", 32);
	winText->setAnchorPoint(Vec2(0.5f, 0.5f));
	_winLabel->addChild(winText, 2);

	auto winSprite = Sprite::create("textures/ui_assets/button_9slice01.png");
	winSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
	_winLabel->addChild(winSprite, 1);

	this->addChild(_winLabel);

	_loseLabel = Node::create();
	_loseLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
	_loseLabel->setPosition(center);
	_loseLabel->setZOrder(5);
	_loseLabel->setVisible(false);

	auto loseText = Label::create("You Lose", "fonts/HVD_Comic_Serif_Pro.otf", 32);
	loseText->setAnchorPoint(Vec2(0.5f, 0.5f));
	_loseLabel->addChild(loseText, 2);

	auto loseSprite = Sprite::create("textures/ui_assets/button_9slice01.png");
	loseSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
	_loseLabel->addChild(loseSprite, 1);

	this->addChild(_loseLabel);

	return true;
}

void GameScene::_writeResult()
{
	auto levels = DataManager::GetInstance()->GetLevelData("json/levels.json");
	levels[_lvlId].isFinish = true;
	DataManager::GetInstance()->SaveLevelData(levels, "json/levels.json");
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
	BuildLevelData data = DataManager::GetInstance()->GetBuildLevelData(src);

	int height = data.height;
	int width = data.width;

	std::vector<std::vector<int>> map = data.matrix;

	_gameField->SetMap(width, height, map);

	_lvlId = data.lvlId;
	_goal = data.goal;
	_moves = data.moves;

}
