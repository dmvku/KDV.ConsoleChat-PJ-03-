#pragma once
#include "user_class.h"
#include "message_class.h"

#include <iostream>
#include <string>
#include <memory>

class Chat
{
public:
	Chat() = default;
	~Chat();	


private:
	std::shared_ptr<User[]> usersInTheChat_{nullptr};
	std::shared_ptr<Message[]> messagesInTheChatt_{nullptr};

	size_t usersMax_{ 5 };
	size_t usersCount_{ 0 };
	size_t messagesMax_{ 25 };
	size_t messagesCount_{ 0 };

};