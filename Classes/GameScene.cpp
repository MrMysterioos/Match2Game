#include "GameScene.h"

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

bool GameScene::init()
{
	if (!Scene::init()) {
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 center(visibleSize.width / 2, visibleSize.height / 2);

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

	_gameField = GameField::create();
	_gameField->setScale(0.4f);
	_gameField->setPosition(center);
	this->addChild(_gameField);

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

	_gameField->Subscribe(this);

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
