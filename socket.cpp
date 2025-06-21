#include "socket.h"
#include <iostream>

Socket::Socket()
{
	port_ = readConnectionConfig();
	if (port_)
	{
		startServer();
	}	
}

Socket::~Socket()
{
	close(connection);
	close(sockert_file_descriptor);
}

void Socket::startServer()
{
	// Создадим сокет
	sockert_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
	if (sockert_file_descriptor == -1)
	{
		std::cout << "Socket creation failed.!\n";
		exit(1);
	}
	// 
	std::cout << "The socket is created...\n";
	serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
	// Зададим номер порта для связи
	serveraddress.sin_port = htons(port_);
	// Используем IPv4
	serveraddress.sin_family = AF_INET;
	// Привяжем сокет
	bind_status = bind(sockert_file_descriptor,
		(struct sockaddr*)&serveraddress,
		sizeof(serveraddress));
	if (bind_status == -1)
	{
		std::cout << "Socket binding failed.!\n";
		exit(1);
	}
	std::cout << "The socket is binded...\n";
	// Поставим сервер на прием данных 
	connection_status = listen(sockert_file_descriptor, 1);
	if (connection_status == -1)
	{
		std::cout << "Socket is unable to listen for new connections.!\n";
		exit(1);
	}
}

void Socket::createConnection()
{
	std::cout << "Server is listening for new connection: \n";
	length = sizeof(client);
	connection = accept(sockert_file_descriptor,
		(struct sockaddr*)&client, &length);
	if (connection == -1)
	{
		std::cout << "Server is unable to accept the data from client.!"
			<< std::endl;
		exit(1);
	}
	std::cout << "The connection is established...\n";
}

void Socket::dataTransmission()
{
	bzero(message, MESSAGE_LENGTH);
	std::copy(sendData_.begin(), sendData_.end(), message);
	ssize_t bytes = write(connection, message, sizeof(message));
	// Если передали >= 0  байт, значит пересылка прошла успешно
	if (bytes >= 0)
	{
		std::cout << "Data successfully sent to the client.!" << message << "\n";
	}	
}

void Socket::dataRecieving()
{
	bzero(message, MESSAGE_LENGTH);
	read(connection, message, sizeof(message));
	std::cout << "The data is received from the client: " << message << "\n";
}

const int Socket::getPort() const
{
	return port_;
}
