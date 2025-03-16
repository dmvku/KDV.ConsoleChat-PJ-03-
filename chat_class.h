#pragma once
#pragma warning(disable : 4996)
#include "user_class.h"
#include "message_class.h"
#include "exception_class.h"
#include "console.h"
#include "trie_struct.h"

//#include <iostream>
//#include <iomanip>
//#include <string>
//#include <vector>
//#include <conio.h>
#include <ctime>
#include <chrono>
//#include <list>

class Chat
{
public:
	Chat() = default;
	~Chat();

	void chatMenu();
	void registerUser();
	void deleteUser();

private:
	void loginUser();
	void newMessage(TrieNode* root);	
	void viewChat();
	void userList();
	list<User>::iterator checkUser(std::string title);
	list<User>::iterator findUser(const std::string& _login);
	bool checkUserName(std::string& to);
	std::string getTheTimeNow();

	std::list<User> chatUsers_;
	std::vector<Message> chatMessages_;	
	list<User>::iterator loginUser_{ chatUsers_.end() };
	int lastCoordinateX_{};
	int lastCoordinateY_{};
	const int linesLimit_{ 16 }; 
};