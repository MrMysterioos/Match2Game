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
	if (!Label::init()) {
		return false;
	}

	_UpdateLabel();

	return true;
}

void GoalLabel::_UpdateLabel()
{
	setString(_title + std::to_string(_current) + '/' + std::to_string(_require));
}
