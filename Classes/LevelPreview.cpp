#include "LevelPreview.h"

bool LevelPreview::init(LevelData data)
{
	if (!Node::init())
		return false;

	_SetLevelData(data);

	return true;
}

void LevelPreview::SetCallback(const ccMenuCallback & callback)
{
	_button->setCallback(callback);
}

void LevelPreview::_SetLevelData(LevelData data)
{
	this->removeAllChildren();
	_button = MenuItemImage::create("textures/ui_assets/button_9slice.png", "textures/ui_assets/button_9slice01.png");
	_button->setAnchorPoint(Vec2(0.5f, 0.5f));
	_button->setZOrder(0);

	auto menu = Menu::create(_button, nullptr);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	_label = Label::create(std::to_string(data.number), "fonts/SnowDream.TTF", 32);
	_label->setAnchorPoint(Vec2(0.5f, 0.5f));
	_label->setZOrder(2);
	this->addChild(_label);

	if (data.isFinish) {
		_checkSprite = Sprite::create("textures/ui_assets/check_mark_small.png");
		_checkSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
		_checkSprite->setPosition(Vec2(-50.f, 40.f));
		_checkSprite->setZOrder(1);
		this->addChild(_checkSprite);
	}
}
