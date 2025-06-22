#pragma once
#include <string>
#include <vector>

#include "socket.h"
#include "user.h"
#include "message.h"

namespace serverCommand
{
	const std::string helloMsg = "#HELLO|";
	const std::string serviceMsg = "#SERVC|";
	const std::string loginMsg = "#LOGIN|";
	const std::string passwordMsg = "#PASSW|";
	const std::string newMessageMsg = "#NEWMS|";
	const std::string beginUserListMsg = "#BULST|";
	const std::string endUserListMsg = "#EULST";
	const std::string beginChatListMsg = "#BCLST|";
	const std::string endChatListMsg = "#ECLST";
	const std::string exitChatMsg = "#EXITC|";
	const std::string confirmationMsg = "#OK";
}

namespace textColor
{
	const std::string resetColor = "\033[0m";
	const std::string black = "\033[30m";
	const std::string red = "\033[31m";
	const std::string green = "\033[32m";
	const std::string yellow = "\033[33m";
	const std::string blue = "\033[34m";
	const std::string magenta = "\033[35m";
	const std::string cyan = "\033[36m";
	const std::string white = "\033[37m";
}

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
