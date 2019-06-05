#include "GameField.h"
#include "ItemSpriteFactory.h"

void GameField::SetMap(int width, int height, std::vector<std::vector<int>> matrix)
{
	_height = height;
	_width = width;

	_offset = Vec2(-(width * _spacing) / 2, -(height * _spacing) / 2);

	_matrix = std::vector<std::vector<Item>>(width, std::vector<Item>(height, Item::Hole));

	for (int ix = 0; ix < width; ix++) {
		for (int iy = 0; iy < height; iy++) {
			if (matrix[ix][iy]) {
				_matrix[ix][iy] = Item::Wall;
			}
			else {
				int n = random() % 7;
				if (n == 0) {
					_matrix[ix][iy] = Item::Bread;
				}
				else if (n == 1) {
					_matrix[ix][iy] = Item::Cocos;
				}
				else if (n == 2) {
					_matrix[ix][iy] = Item::Milk;
				}
				else if (n == 3) {
					_matrix[ix][iy] = Item::Orange;
				}
				else if (n == 4) {
					_matrix[ix][iy] = Item::Tomato;
				}
				else if (n == 5) {
					_matrix[ix][iy] = Item::Broccoli;
				}
				else if (n == 6) {
					_matrix[ix][iy] = Item::Cristal;
				}
			}
		}
	}

	_isLoaded = true;

	_CreateGrid();

	_CreateItems();
}

void GameField::EnableInput()
{
	_eventDispatcher->setEnabled(true);
}

void GameField::DisableInput()
{
	_eventDispatcher->setEnabled(false);
}

bool GameField::init() {
	if (!Node::init()) {
		return false;
	}

	EventListenerMouse* _mouseListener = EventListenerMouse::create();
	_mouseListener->onMouseDown = [&](Event* event) {
		EventMouse* mouseEvent = (EventMouse*)event;

		Vec2 co = _GetPointedCellCoordinates(mouseEvent->getLocation());
		log("mouse_down (%i : %i)", (int)co.x, (int)co.y);

		if (mouseEvent->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT) {
			if (co.x > 0 && co.x < _width && co.y > 0 && co.y < _height) {
				_pos1 = co;
			}
		}

	};
	_mouseListener->onMouseUp = [&](Event* event) {
		EventMouse* mouseEvent = (EventMouse*)event;
		Vec2 co = _GetPointedCellCoordinates(mouseEvent->getLocation());
		log("mouse_up (%i : %i)", (int)co.x, (int)co.y);

		if (mouseEvent->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT) {

			if (co.x > 0 && co.x < _width && co.y > 0 && co.y < _height) {
				_pos2 = co;
			}

			if (_gameState == GameState::Turn && _pos1 != Vec2(-1.f, -1.f) && _pos2 != Vec2(-1.f, -1.f)) {
				_Swap(_pos1, _pos2);
				_gameState = GameState::Swaping;
				_timeBeforeUpdate = 1.f / _gameSpeed;
				Send("Swap", "");
			}

			_pos1 = _pos2 = Vec2(-1.f, -1.f);
		}

	};

	_eventDispatcher->addEventListenerWithFixedPriority(_mouseListener, 1);

	scheduleUpdate();

	return true;
}

void GameField::update(float dt)
{
	float timeReserve = 0.05f;
	if (_timeBeforeUpdate + timeReserve > 0.f) {
		_timeBeforeUpdate -= dt;
		if (_timeBeforeUpdate + timeReserve <= 0) {
			_updateState();
		}
	}
}

void GameField::_Swap(Vec2 first, Vec2 second)
{
	std::swap(_matrix[round(first.x)][round(first.y)], _matrix[round(second.x)][round(second.y)]);

	if (_sprites[first]) {
		Action* move1 = MoveTo::create(1.f / _gameSpeed, (Vec2(second.x, second.y) * _spacing) + _offset);
		_sprites[first]->stopAllActions();
		_sprites[first]->runAction(move1);
	}
	if (_sprites[second]) {
		Action* move2 = MoveTo::create(1.f / _gameSpeed, (Vec2(first.x, first.y) * _spacing) + _offset);
		_sprites[second]->stopAllActions();
		_sprites[second]->runAction(move2);
	}

	std::swap(_sprites[first], _sprites[second]);
	
}

void GameField::_Destroy(Vec2 item)
{
	if (!_sprites[item]) {
		return;
	}

	_matrix[round(item.x)][round(item.y)] = Item::Hole;

	auto fadeOut = FadeOut::create(1.f / _gameSpeed);

	auto sprite = _sprites[item];
	auto callback = CallFunc::create([=]() {
		sprite->removeFromParent();
	});

	_sprites[item]->runAction(Sequence::create(fadeOut, callback, nullptr));
	_sprites.erase(item);

}

void GameField::_CreateItem(Vec2 coord, Item type)
{
	_matrix[round(coord.x)][round(coord.y)] = type;
	Sprite* sprite = ItemSpriteFactory::CreateItemSprite(type);

	sprite->setPosition(Vec2(coord.x * _spacing, (coord.y + 1.f) * _spacing) + _offset);
	sprite->setAnchorPoint(Vec2::ZERO);

	auto fadeIn = FadeIn::create(1.f / _gameSpeed);
	auto moveTo = MoveTo::create(1.f / _gameSpeed, Vec2(coord.x * _spacing, coord.y * _spacing) + _offset);
	sprite->setOpacity(0);
	sprite->runAction(Spawn::create(fadeIn, moveTo, nullptr));

	this->addChild(sprite);
	_sprites[coord] = sprite;
}

void GameField::_CreateRandomItem(Vec2 coord)
{
	int r = random() % 7;
	Item type;
	if (r == 0) {
		type = Item::Cocos;
	}
	else if (r == 1) {
		type = Item::Milk;
	}
	else if (r == 2) {
		type = Item::Orange;
	}
	else if (r == 3) {
		type = Item::Bread;
	}
	else if (r == 4) {
		type = Item::Tomato;
	}
	else if (r == 5) {
		type = Item::Cristal;
	}
	else if (r == 6) {
		type = Item::Broccoli;
	}
	_CreateItem(coord, type);
}

bool GameField::_UpdateWaterfall()
{
	bool res = false;
	for (int iy = 0; iy < _height - 1; iy++) {
		for (int ix = 0; ix < _width; ix++) {
			if (_matrix[ix][iy] == Item::Hole) {
				if (_matrix[ix][iy + 1] != Item::Hole && _matrix[ix][iy + 1] != Item::Wall) {
					_Swap(Vec2(ix, iy + 1), Vec2(ix, iy));
					res = true;
				}
			}
		}
	}
	return res;
}

void GameField::_AddNewItems()
{
	for (int ix = 0; ix < _width; ix++) {
		if (_matrix[ix][_height - 1] == Item::Hole) {
			_CreateRandomItem(Vec2(ix, _height - 1));
		}
	}

}

int GameField::_CheckMatch() {
	int points = 0;

	for (int iy = 0; iy < _height; iy++) {
		int matchCount = 1;
		Item lastItem = Item::Hole;
		for (int ix = 0; ix < _width; ix++) {
			if (lastItem != Item::Wall && lastItem != Item::Hole && lastItem == _matrix[ix][iy]) {
				++matchCount;
			}
			else {
				if (matchCount >= 2) {
					for (int i = 0; i < matchCount; ++i) {
						_Destroy(Vec2(ix - (i + 1), iy));
						points++;
					}
				}
				lastItem = _matrix[ix][iy];
				matchCount = 1;
			}
		}
		if (matchCount >= 2) {
			for (int i = 0; i < matchCount; ++i) {
				_Destroy(Vec2(_width - (i + 1), iy));
				points++;
			}
		}
	}

	for (int ix = 0; ix < _height; ix++) {
		int matchCount = 1;
		Item lastItem = Item::Hole;
		for (int iy = 0; iy < _width; iy++) {
			if (lastItem != Item::Wall && lastItem != Item::Hole && lastItem == _matrix[ix][iy]) {
				++matchCount;
			}
			else {
				if (matchCount >= 2) {
					for (int i = 0; i < matchCount; ++i) {
						_Destroy(Vec2(ix, iy - (i + 1)));
						points++;
					}
				}
				lastItem = _matrix[ix][iy];
				matchCount = 1;
			}
		}
		if (matchCount >= 2) {
			for (int i = 0; i < matchCount; ++i) {
				_Destroy(Vec2(ix, _height - (i + 1)));
				points++;
			}
		}
	}

	Send("Match", std::to_string(points));
	return points;
}

void GameField::_CreateGrid()
{
	if (!_isLoaded) {
		// Error
		return;
	}

	for (int ix = 0; ix < _width; ix++) {
		for (int iy = 0; iy < _height; iy++) {
			Sprite* sprite = Sprite::create("textures/characters/grid_0001.png");
			sprite->setPosition(Vec2(ix * _spacing, iy * _spacing) + _offset);
			sprite->setAnchorPoint(Vec2(0.f, 0.f));
			this->addChild(sprite);
		}
	}

}

void GameField::_CreateItems() {
	if (!_isLoaded) {
		// Error
		return;
	}

	for (int ix = 0; ix < _width; ix++) {
		for (int iy = 0; iy < _height; iy++) {

			Sprite* sprite = ItemSpriteFactory::CreateItemSprite(_matrix[ix][iy]);
			sprite->setPosition(Vec2(ix * _spacing, iy * _spacing) + _offset);
			sprite->setAnchorPoint(Vec2::ZERO);
			this->addChild(sprite);
			_sprites.insert(std::pair<Vec2, Sprite*>(Vec2(ix, iy), sprite));

		}
	}

}

Vec2 GameField::_GetPointedCellCoordinates(Vec2 mouseCoord)
{
	Vec2 origin = _position + Vec2(_offset.x * _scaleX, _offset.y * _scaleY);
	Vec2 localMouseCoord = mouseCoord - origin;
	Size cellSize = Size(_spacing * _scaleX, _spacing * _scaleY);
	int x = (int)localMouseCoord.x / cellSize.width;
	int y = _height - (int)localMouseCoord.y / cellSize.height;
	return Vec2(x, y);
}

void GameField::_updateState()
{
	if (_gameState == GameState::Swaping) {
		if (_CheckMatch()) {
			_AddNewItems();
			_gameState = GameState::Match;
			_timeBeforeUpdate = 1.f / _gameSpeed;
		}
		else {
			Send("Stability", "");
			_gameState = GameState::Turn;
		}
		
	}
	else if (_gameState == GameState::Match) {
		if (_UpdateWaterfall()) {
			_AddNewItems();
			_gameState = GameState::Waterfall;
			_timeBeforeUpdate = 1.f / _gameSpeed;
		}
		else if (_CheckMatch()) {
			_AddNewItems();
			_gameState = GameState::Match;
			_timeBeforeUpdate = 1.f / _gameSpeed;
		}
		else {
			Send("Stability", "");
			_gameState = GameState::Turn;
		}
	}
	else if (_gameState == GameState::Waterfall) {
		if (_UpdateWaterfall()) {
			_AddNewItems();
			_timeBeforeUpdate = 1.f / _gameSpeed;
		}
		else if (_CheckMatch()) {
			_AddNewItems();
			_gameState = GameState::Match;
			_timeBeforeUpdate = 1.f / _gameSpeed;
		}
		else {
			Send("Stability", "");
			_gameState = GameState::Turn;
		}
	}
}

void GameField::CreateRandomMap(int width, int height)
{
	_height = height;
	_width = width;

	_offset = Vec2(-(width * _spacing) / 2, -(height * _spacing) / 2);

	_matrix = std::vector<std::vector<Item>>(width, std::vector<Item>(height, Item::Hole));

	for (int ix = 0; ix < width; ix++) {
		for (int iy = 0; iy < height; iy++) {
			int n = random() % 10;
			if (n == 0) {
				_matrix[ix][iy] = Item::Bread;
			}
			else if (n == 1) {
				_matrix[ix][iy] = Item::Cocos;
			}
			else if (n == 2) {
				_matrix[ix][iy] = Item::Milk;
			}
			else if (n == 3) {
				_matrix[ix][iy] = Item::Orange;
			}
			else if (n == 4) {
				_matrix[ix][iy] = Item::Tomato;
			}
			else if (n == 5) {
				_matrix[ix][iy] = Item::Broccoli;
			}
			else if (n == 6) {
				_matrix[ix][iy] = Item::Cristal;
			}
			else{
				_matrix[ix][iy] = Item::Wall;
			}
		}
	}

	_isLoaded = true;

	_CreateGrid();

	_CreateItems();
}
