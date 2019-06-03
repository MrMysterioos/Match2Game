#include "MessageSender.h"

void MessageSender::Subscribe(MessageListener* listener)
{
	listeners.push_back(listener);
}

void MessageSender::Send(std::string message, std::string data)
{
	for (int i = 0; i < listeners.size(); i++) {
		listeners[i]->AcceptMessage(message, data);
	}
}
