#pragma once

#include "cocos2d.h"
#include "GameField.h"
#include "MessageListener.h"
#include "MovesLeftLabel.h"
#include "GoalLabel.h"

USING_NS_CC;

class GameScene : public Scene, public MessageListener {
public:

	enum GameSceneState { Play, Win, Lose };

	virtual void AcceptMessage(std::string message, std::string data);

	virtual bool init();

	CREATE_FUNC(GameScene);

private:

	void _setState(GameSceneState newState);

	GameSceneState _state;

	GameField* _gameField;
	MovesLeftLabel* _mll;
	GoalLabel* _gl;

	Node* _winLabel;
	Node* _loseLabel;

	int _moves = 25;
	int _goal = 100;
	int _points = 0;

	// temp
	void _ShowCoordinatesSystem(int step);

};