#pragma once

#include <string>
#include <vector>
// #include <memory>

#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include <netinet/in.h>

#include "user_class.h"
#include "message_class.h"

#define MESSAGE_LENGTH 1024 // Максимальный размер буфера для данных

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
	Chat(int port);
	~Chat();

	void runChat();

private:	
	struct sockaddr_in serveraddress, client;
	socklen_t length;
	int sockert_file_descriptor, connection, bind_status, connection_status;
	char message[MESSAGE_LENGTH];
	int _port{};
	
	std::vector<User> chatUsers_;
	std::vector<Message> chatMessages_;
	std::string loginUser_;
	std::string sendData_;

	std::string usersFile_{ "users.data" };
	std::string messagesFile_{ "messages.data" };
	
	void startServer();
	void createConnection();	
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
	void dataTransmission();
	void dataRecieving();
	std::string dataParsing(std::string& data);
};
