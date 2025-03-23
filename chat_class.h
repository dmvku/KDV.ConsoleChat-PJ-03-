#pragma once
#pragma warning(disable : 4996)
#include "user_class.h"
#include "message_class.h"
#include "exception_class.h"
#include "console.h"
#include "trie_struct.h"

#include <ctime>
#include <chrono>

class Chat
{
public:
	Chat() = default;
	~Chat();

	void mainMenu();

	int lastCoordinateX_{};
	int lastCoordinateY_{};

private:
	void chatMenu();
	void registerUser();
	void deleteUser();
	void loginUser();
	void newMessage(TrieNode* root);	
	void viewChat();
	void userList();
	std::list<User>::iterator checkUser(std::string title);
	std::list<User>::iterator findUser(const std::string& _login);
	bool checkUserName(std::string& to);
	std::string getTheTimeNow();

	std::list<User> chatUsers_;
	std::vector<Message> chatMessages_;	
	std::list<User>::iterator loginUser_{ chatUsers_.end() };
	//int lastCoordinateX_{};
	//int lastCoordinateY_{};
	const int linesLimit_{ 16 }; 
};
