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

class Chat
{
public:
	Chat() = default;
	~Chat();

	void chatMenu();
	void registerUser();

private:
	void loginUser();
	void newMessage(TrieNode* root);
	void viewChat();
	void userList();
	bool checkUserLogin(std::string& to);
	bool checkUserName(std::string& to);
	std::string getTheTimeNow();

	std::vector<User> chatUsers_;
	std::vector<Message> chatMessages_;	
	std::shared_ptr<User> loginUser_ { nullptr };	
	int lastCoordinateX_{};
	int lastCoordinateY_{};
	const int linesLimit_{ 16 }; 
};