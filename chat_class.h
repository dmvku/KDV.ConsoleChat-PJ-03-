#pragma once

#include <string>
#include <vector>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "connection_config.h"
#include "user_class.h"
#include "message_class.h"

#define MESSAGE_LENGTH 1024 // Максимальный размер буфера для данных

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

	void startClient();	

private:	
	ConnectionConfig serverConfig_;
	int socket_file_descriptor, connection;
	struct sockaddr_in serveraddress, client;
	char message[MESSAGE_LENGTH];
	
	int serverPort_{};
	std::string serverAddress_;

	User currentUser_;
	std::vector<User> chatUsers_;
	std::vector<Message> chatMessages_;	
	std::string sendData_;
	
	void configuringTheServerConnection();	
	void createSocket();
	void connectionToTheServer();
	void interactionWithTheServer();
	void createMessage(std::string& newMessage);
	void viewChat();
	void viewUsers();
	void dataTransmission();
	void dataRecieving();
	void recivingChatData(char list);
	std::string dataParsing(std::string& data);	
};

