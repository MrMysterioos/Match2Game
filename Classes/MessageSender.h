#pragma once

#include "cocos2d.h"
#include "MessageListener.h"

class MessageSender {
public:
	void Subscribe(MessageListener* listener);
protected:
	void Send(std::string message, std::string data);
private:
	std::vector<MessageListener*> listeners;
};