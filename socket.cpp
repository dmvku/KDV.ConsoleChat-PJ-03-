#include "socket.h"
#include <iostream>
#include <unistd.h>

Socket::Socket()
{
	setPort();
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
		std::cout << socketLogColor::red 
			<< "Socket creation failed.!\n"
			<< socketLogColor::resetColor;
		exit(1);
	}
	// 
	std::cout << socketLogColor::green 
		<< "The socket is created...\n" 
		<< socketLogColor::resetColor;
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
		std::cout << socketLogColor::red
			<< "Socket binding failed.!\n"
			<< socketLogColor::resetColor;
		exit(1);
	}
	std::cout << socketLogColor::green 
		<< "The socket is binded...\n"
		<< socketLogColor::resetColor;
	// Поставим сервер на прием данных 
	connection_status = listen(sockert_file_descriptor, 5);
	if (connection_status == -1)
	{
		std::cout << socketLogColor::red
			<< "Socket is unable to listen for new connections.!\n"
			<< socketLogColor::resetColor;
		exit(1);
	}
}

void Socket::stopServer()
{
	close(connection);
	close(sockert_file_descriptor);
}

void Socket::createConnection()
{
	std::cout << "Server is listening for new connection: \n";
	length = sizeof(client);
	connection = accept(sockert_file_descriptor,
		(struct sockaddr*)&client, &length);
	if (connection == -1)
	{
		std::cout << socketLogColor::red
			<< "Server is unable to accept the data from client.! \n"
			<< socketLogColor::resetColor;
		exit(1);
	}
	std::cout << socketLogColor::green
		<< "The connection is established...\n"
		<< socketLogColor::resetColor;
}

void Socket::closeConnection()
{
	close(connection);
}

void Socket::dataTransmission()
{
	// Time delay for client readiness
	usleep(100000); 
	bzero(message, MESSAGE_LENGTH);
	std::copy(sendData_.begin(), sendData_.end(), message);
	ssize_t bytes = write(connection, message, sizeof(message));	
	if (bytes >= 0)
	{
		socketSendResultLog_ = "Done.";
		/*std::cout << socketLogColor::green
			<< "Sent: " << socketLogColor::resetColor
			<< message << "\n";*/
	}	
}

void Socket::dataRecieving()
{
	bzero(message, MESSAGE_LENGTH);
	read(connection, message, sizeof(message));
	/*std::cout << socketLogColor::yellow 
		<< "Received: " << socketLogColor::resetColor
		<< message << "\n";*/
}

const int Socket::getPort() const
{
	return port_;
}

void Socket::setPort()
{
	port_ = serverConfig.getPort();
}
