#include "GoalLabel.h"

void GoalLabel::SetCurrentValue(int newValue)
{
	_current = newValue;
	_UpdateLabel();
}

void GoalLabel::SetRequireValue(int newValue)
{
	_require = newValue;
	_UpdateLabel();
}

bool GoalLabel::init()
{
	if (!Node::init()) {
		return false;
	}

	_label = Label::create(_title, "fonts/HVD_Comic_Serif_Pro.otf", 32);
	_label->setAnchorPoint(Vec2(0.f, 1.f));
	this->addChild(_label, 1);

	auto back = Sprite::create("textures/ui_assets/inGameCounter_bar.png");
	back->setAnchorPoint(Vec2(0.f, 0.8f));
	this->addChild(back, 0);

	_UpdateLabel();

	return true;
}

void GoalLabel::_UpdateLabel()
{
	_label->setString(_title + std::to_string(_current) + '/' + std::to_string(_require));
}
