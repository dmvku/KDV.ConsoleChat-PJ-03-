#pragma once
#include "user_class.h"
#include "message_class.h"
#include "exception_class.h"

#include <iostream>
#include <string>
#include <vector>
#include <conio.h>


using namespace std;

class Chat
{
public:
	Chat() = default;
	~Chat();

	void registerUser();
	void loginUser();
	void chatMenu();
	void newMessage();
	void viewChat();
	void userList();
	//void addMessage(std::string& to, bool privateMessage);
	bool checkUserLogin(std::string& to);
	bool checkUserName(std::string& to);

	std::string checkLogin();



private:
	std::vector<User> chatUsers_;
	std::vector<Message> chatMessages_;	
	std::shared_ptr<User> loginUser_ { nullptr };

};