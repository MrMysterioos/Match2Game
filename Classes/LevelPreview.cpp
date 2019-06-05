#include "LevelPreview.h"
#include "GameScene.h"

bool LevelPreview::init(LevelData data)
{
	if (!Node::init())
		return false;

	_SetLevelData(data);

	return true;
}

void LevelPreview::_SetLevelData(LevelData data)
{
	this->removeAllChildren();
	
	_button = MenuItemImage::create("textures/ui_assets/button_9slice.png", "textures/ui_assets/button_9slice01.png", [=](Ref* pSender) {
		log("click");
		GameScene* gameScene = GameScene::create(data.source);
		Director::getInstance()->replaceScene(gameScene);
	});
	_button->setAnchorPoint(Vec2(0.5f, 0.5f));
	_button->setZOrder(0);

	auto menu = Menu::create(_button, nullptr);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	/*
	_mainSprite = Sprite::create("textures/ui_assets/iconLevel.png");
	_mainSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
	_mainSprite->setZOrder(0);
	this->addChild(_mainSprite);
	*/

	_label = Label::create(std::to_string(data.number), "SnowDream.TTF", 32);
	_label->setAnchorPoint(Vec2(0.5f, 0.5f));
	_label->setZOrder(2);
	this->addChild(_label);

	if (data.isFinish) {
		_checkSprite = Sprite::create("textures/ui_assets/check_mark.png");
		_checkSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
		_checkSprite->setZOrder(1);
		this->addChild(_checkSprite);
	}
}
