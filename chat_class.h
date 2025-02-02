#pragma once
#include "user_class.h"
#include "message_class.h"
#include "exception_class.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Chat
{
public:
	Chat() = default;
	~Chat();

	void registerUser();
	void loginUser();
	void chatMenu();

	std::string checkLogin();



private:
	std::vector<User> chatUsers_;
	std::vector<Message> chatMessages_;	
	std::shared_ptr<User> loginUser_ { nullptr };

};