#pragma once

#include "cocos2d.h"

USING_NS_CC;

class MovesLeftLabel : public Node {
public:
	void SetValue(int newValue);

	CREATE_FUNC(MovesLeftLabel);

private:
	bool init() override;

	void _UpdateLabel();

	Label* _label;

	std::string _title = "Moves left: ";
	int _moves = 25;
};