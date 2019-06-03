#include "MovesLeftLabel.h"

void MovesLeftLabel::SetValue(int newValue)
{
	_moves = newValue;
	_UpdateLabel();
}

void MovesLeftLabel::_UpdateLabel()
{
	setString(_title + std::to_string(_moves));
}

bool MovesLeftLabel::init()
{
	if (!Label::init()) {
		return false;
	}

	_UpdateLabel();

	return true;
}
