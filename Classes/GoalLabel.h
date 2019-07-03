#pragma once

#include "cocos2d.h"

USING_NS_CC;

class GoalLabel : public Node {
public:
	void SetCurrentValue(int newValue);
	void SetRequireValue(int newValue);

	CREATE_FUNC(GoalLabel);
private:
	bool init() override;

	void _UpdateLabel();

	Label* _label;

	std::string _title = "Goal:";
	int _require = 100;
	int _current = 0;
};