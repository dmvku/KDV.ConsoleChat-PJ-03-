#pragma once

#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "connection_config.h"

#define MESSAGE_LENGTH 1024 // Максимальный размер буфера для данных

class Socket
{
public:
	ConnectionConfig serverConfig_;
	struct sockaddr_in serveraddress;
	struct sockaddr_in client;
	int socket_file_descriptor;
	int connection;
	
	char message[MESSAGE_LENGTH];
	std::string sendData_;

	Socket();
	~Socket();
	void startClient();
	void configuringTheServerConnection();	
	void dataTransmission();
	void dataRecieving();
	
private:
	int serverPort_{};
	std::string serverAddress_;
};
