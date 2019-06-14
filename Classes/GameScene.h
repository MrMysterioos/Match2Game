#pragma once

#include "cocos2d.h"
#include "GameField.h"
#include "MessageListener.h"
#include "MessageSender.h"
#include "MovesLeftLabel.h"
#include "GoalLabel.h"

USING_NS_CC;

class GameScene : public Scene, public MessageListener, public MessageSender {
public:

	enum GameSceneState { Play, Win, Lose };

	virtual void AcceptMessage(std::string message, std::string data);

	virtual bool init(std::string src);

	static GameScene* create(std::string src)
	{
		GameScene *pRet = new(std::nothrow) GameScene();
		if (pRet && pRet->init(src))
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			delete pRet;
			pRet = nullptr;
			return nullptr;
		}
	}

private:

	void _writeResult();

	void _setState(GameSceneState newState);
	void _loadLvl(std::string src);

	GameSceneState _state;

	GameField* _gameField;
	MovesLeftLabel* _mll;
	GoalLabel* _gl;

	Node* _winLabel;
	Node* _loseLabel;

	int _moves = 25;
	int _goal = 100;
	int _points = 0;

	int _lvlId;

	// temp
	void _ShowCoordinatesSystem(int step);

};