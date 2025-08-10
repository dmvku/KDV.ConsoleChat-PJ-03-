#pragma once
#include "server_config.h"

#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <vector>

#define MESSAGE_LENGTH 1024 // Максимальный размер буфера для данных

namespace socketLogColor
{
	const std::string resetColor = "\033[0m";
	const std::string red = "\033[31m";
	const std::string green = "\033[32m";
	const std::string yellow = "\033[33m";
}

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
	std::string socketSendResultLog_;
	
	void startServer();
	void stopServer();
	void createConnection();
	void closeConnection();
	void dataTransmission();
	void dataRecieving();	
	const int getPort() const;

private:
	ServerConfig serverConfig;
	int port_{};

	void setPort();
};