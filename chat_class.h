#pragma once
#include "user_class.h"
#include "message_class.h"
#include "exception_class.h"

#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

class Chat
{
public:
	Chat() = default;
	~Chat();	

	void mainMenu();	

private:
	void registerUser();
	void loginUser();
	void chatMenu();
	void newMessage();
	void viewChat();
	void userList();	
	bool checkUserLogin(std::string& to);
	bool checkUserName(std::string& to);
	void readUsersFile();
	void readMessagesFile();

	std::vector<User> chatUsers_;
	std::vector<Message> chatMessages_;
	std::shared_ptr<User> loginUser_{ nullptr };

	std::string usersFile_{ "users.data" };
	std::string messagesFile_{ "messages.data" };

	//std::string checkLogin();


	

};