#pragma once

#include "cocos2d.h"

class MessageListener {
public:
	virtual void AcceptMessage(std::string message, std::string data) = 0;
private:

};