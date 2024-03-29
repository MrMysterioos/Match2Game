#pragma once

#include "cocos2d.h"
#include "Items.h"
#include "MessageSender.h"

USING_NS_CC;

class GameField : public Node, public MessageSender {
public:

	enum GameState { Turn, Match, Waterfall, Swapping, Returning };

	void SetMap(int width, int height, std::vector<std::vector<int>> matrix);
	void CreateRandomMap(int width, int height);

	void EnableInput();
	void DisableInput();

	int GetHeight() { return _height; };
	int GetWidth() { return _width; };

	Size GetSize() { return Size(_width * _spacing, _height * _spacing); };

	CREATE_FUNC(GameField);

private:

	virtual bool init();

	virtual void update(float dt) override;

	void _Swap(Vec2 first, Vec2 second);
	bool _Destroy(Vec2 coord);
	void _CreateItem(Vec2 coord, Item type);
	void _CreateRandomItem(Vec2 coord);

	bool _UpdateWaterfall();
	void _AddNewItems();

	int _CheckMatch();

	void _CreateGrid();

	void _CreateItems();

	Vec2 _GetPointedCellCoordinates(Vec2 mouseCoord);

	void _updateState();

	void _SelectSprite(Vec2 coords);
	void _UnselectSprite();

	~GameField();

	EventListenerMouse* _mouseListener;

	GameState _gameState = GameState::Turn;

	bool _isLoaded = false;

	float _gameSpeed = 4;

	float _timeBeforeUpdate = 0.f;

	int _width = 0;
	int _height = 0;

	Vec2 _offset;

	int _spacing = 254;

	std::vector<std::vector<Item>> _matrix;

	std::map<Vec2, Sprite*> _sprites;

	Sprite* _selectedSprite = nullptr;

	// last swap

	Vec2 _lastPos1 = Vec2(-1.f, -1.f);
	Vec2 _lastPos2 = Vec2(-1.f, -1.f);

	// control

	bool _drag = false;

	Vec2 _pos1 = Vec2(-1.f, -1.f);
	Vec2 _pos2 = Vec2(-1.f, -1.f);

};