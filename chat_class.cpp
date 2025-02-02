#include "chat_class.h"

Chat::Chat()
{	
	auto usersInTheChat_{ std::make_shared<User[]>(usersMax_) };
	auto messagesInTheChat_{ std::make_shared<Message[]>(messagesMax_) };
}

Chat::~Chat()
{

}

