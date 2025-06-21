#pragma once

#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <vector>

#include "connection_config.h"

#define MESSAGE_LENGTH 1024 // Максимальный размер буфера для данных

class Socket
{
public:
	Socket();
	~Socket();
	
	struct sockaddr_in serveraddress;
	struct sockaddr_in client;
	socklen_t length;
	int sockert_file_descriptor;
	int connection;
	int bind_status;
	int connection_status;

	char message[MESSAGE_LENGTH];	
	std::string sendData_;
	
	void startServer();
	void createConnection();
	void dataTransmission();
	void dataRecieving();	
	const int getPort() const;

private:
	int port_{};
};