#pragma once

#include "cocos2d.h"
#include "Items.h"
#include "MessageSender.h"

USING_NS_CC;

class GameField : public Node, public MessageSender {
public:

	enum GameState { Turn, Match, Waterfall, Swaping };

	void EnableInput();
	void DisableInput();

	CREATE_FUNC(GameField);

private:

	virtual bool init();

	virtual void update(float dt) override;

	void _Swap(Vec2 first, Vec2 second);
	void _Destroy(Vec2 coord);
	void _CreateItem(Vec2 coord, Item type);
	void _CreateRandomItem(Vec2 coord);

	bool _UpdateWaterfall();
	void _AddNewItems();

	int _CheckMatch();

	void _CreateGrid();

	void _CreateItems();

	Vec2 _GetPointedCellCoordinates(Vec2 mouseCoord);

	void _updateState();

	GameState _gameState = GameState::Turn;

	bool _isLoaded = false;

	float _gameSpeed = 2;

	float _timeBeforeUpdate = 0.f;

	int _width = 0;
	int _height = 0;

	Vec2 _offset;

	int _spacing = 254;

	std::vector<std::vector<Item>> _matrix;

	std::map<Vec2, Sprite*> _sprites;

	// control

	Vec2 _pos1;
	Vec2 _pos2;

	// temp

	void _createRandomData(int width, int height);
};