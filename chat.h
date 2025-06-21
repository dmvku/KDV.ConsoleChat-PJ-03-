#pragma once

#include <string>
#include <vector>

#include "socket.h"
#include "user.h"
#include "message.h"

constexpr const char* serviceMsg = "#SERVC|";
constexpr const char* loginMsg = "#LOGIN|";
constexpr const char* passwordMsg = "#PASSW|";
constexpr const char* newMessageMsg = "#NEWMS|";
constexpr const char* beginUserListMsg = "#BULST|";
constexpr const char* endUserListMsg = "#EULST";
constexpr const char* beginChatListMsg = "#BCLST|";
constexpr const char* endChatListMsg = "#ECLST";
constexpr const char* exitChatMsg = "#EXITC|";

class Chat
{
public:
	Chat();
	~Chat();

	void runChat();

private:		
	Socket server;
	std::vector<User> chatUsers_;
	std::vector<Message> chatMessages_;
	std::string loginUser_;	
	std::string usersFile_{ "users.data" };
	std::string messagesFile_{ "messages.data" };
	
	bool userAutorization();
	bool registerUser();
	bool loginUser();
	void sendChatData();
	void sendUserList();
	void sendMessages();
	bool chatMenu();
	void newMessage();	
	bool checkUserLogin(std::string& login);
	bool checkUserName(std::string& name);
	void readDataFile(std::string& file);
	void readUsersFile(std::string& line);
	void readMessagesFile(std::string& line);	
	std::string dataParsing(std::string& data);

	// void viewChat();
	// void userList();
	// bool requestProcessing(std::string& data);
};
