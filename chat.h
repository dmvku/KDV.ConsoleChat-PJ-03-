#pragma once

#include <string>
#include <vector>

#include "socket.h"
#include "user.h"
#include "message.h"

constexpr const char* serviceMsg = "#SERVC";
constexpr const char* loginMsg = "#LOGIN";
constexpr const char* passwordMsg = "#PASSW";
constexpr const char* newMessageMsg = "#NEWMS";
constexpr const char* beginUserListMsg = "#BULST";
constexpr const char* endUserListMsg = "#EULST";
constexpr const char* beginChatListMsg = "#BCLST";
constexpr const char* endChatListMsg = "#ECLST";
constexpr const char* exitChatMsg = "#EXITC";

class Chat
{
public:
	Chat();
	~Chat();

	void startChat();
	
private:
	Socket client;
	User currentUser_;
	std::vector<User> chatUsers_;
	std::vector<Message> chatMessages_;	
	
	void createMessage(std::string& newMessage);
	void viewChat();
	void viewUsers();
	void recivingChatData(char list);
	std::string dataParsing(std::string& data);	
};
