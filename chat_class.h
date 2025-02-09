#pragma once
#pragma warning(disable : 4996)
#include "user_class.h"
#include "message_class.h"
#include "exception_class.h"
#include "console.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <conio.h>
#include <ctime>
#include <chrono>

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
	bool checkUserLogin(std::string& to);
	bool checkUserName(std::string& to);
	std::string getTheTimeNow();

private:
	std::vector<User> chatUsers_;
	std::vector<Message> chatMessages_;	
	std::shared_ptr<User> loginUser_ { nullptr };	
	int lastCoordinateX_{};
	int lastCoordinateY_{};
	const int linesLimit_{ 16 }; 
};