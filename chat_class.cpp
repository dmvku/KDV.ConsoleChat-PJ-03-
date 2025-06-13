#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>

#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "chat_class.h"
#include "connection_config.h"
#include "user_class.h"
#include "message_class.h"
#include "exception_class.h"

Chat::Chat(int port) : _port(port)
{    
	startServer();
	readDataFile(usersFile_);
	readDataFile(messagesFile_);
}

Chat::~Chat()
{
	close(sockert_file_descriptor);
}

void Chat::startServer()
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
	serveraddress.sin_port = htons(_port);
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

void Chat::createConnection()
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

void Chat::runChat()
{
	std::cout << "Run chat...\n";

	while (1)
	{
		createConnection();		
				
		// Call to register or login
		if (!userAutorization())
		{
			std::cout << "End chat...\n";
			close(sockert_file_descriptor);
			return;
		}

		sendChatData();

		if (!chatMenu())
		{
			std::cout << "End chat...\n";
			close(sockert_file_descriptor);
			return;
		}
		else
		{
			std::cout << "Logout...\n";
			close(connection);
		}
	}	
}

bool Chat::userAutorization()
{
	std::cout << "Register...\n";
	sendData_ = "Hello! Please login(l), register (r) or exit(other key)...\n";
	dataTransmission();
	dataRecieving();
		
	switch (message[0])	
	{
	case 'l':
		if (chatUsers_.empty())
		{
			sendData_ = serviceMsg
				+ std::string("Users not found. Please register...\n");
			dataTransmission();
			return registerUser();
		}
		else
		{
			return loginUser();
		}		
	case 'r':
		return registerUser();		
	default:
		sendData_ = exitChatMsg;
		dataTransmission();
		return 0;
	}
}

bool Chat::registerUser()
{	
	std::string login;
	std::string passwordHash;
	std::string name;
	bool valueIsBusy{};
	std::cout << "Register user...\n";
	
	do
	{
		valueIsBusy = false;
		sendData_ = "Login: \0";
		dataTransmission();		
		dataRecieving();
		login = std::string(message);

		try
		{
			if (checkUserLogin(login))
			{
				throw Warning();
			}
		}
		catch (std::exception& warning)
		{			
			sendData_ = serviceMsg + std::string(warning.what())
				+ std::string("login is busy. Сhoose a different login...\n");
			std::cout << sendData_;
			dataTransmission();
			valueIsBusy = true;
			break;
		}	
	} while (valueIsBusy);
	
	sendData_ = passwordMsg + std::string("Password: ");
	dataTransmission();
	dataRecieving();
	passwordHash = std::string(message);

	do
	{
		valueIsBusy = false;
		sendData_ = "Name: ";
		dataTransmission();
		dataRecieving();
		name = std::string(message);
		try
		{
			if (checkUserName(name))
			{
				throw Warning();
			}
		}
		catch (std::exception& warning)
		{
			sendData_ = serviceMsg + std::string(warning.what())
				+ std::string("name is busy. Сhoose a different name...\n");
			std::cout << sendData_;			
			dataTransmission();			
			valueIsBusy = true;
		}
	} while (valueIsBusy);

	chatUsers_.push_back(User{ login, passwordHash, name });

	std::ofstream fileStream(usersFile_, std::ios::app);	
	if (!fileStream)
	{
		fileStream = std::ofstream(usersFile_, std::ios::out | std::ios::app);
		std::filesystem::permissions(messagesFile_,
			std::filesystem::perms::group_all | std::filesystem::perms::others_all,
			std::filesystem::perm_options::remove);
	}
	else
	{
		std::string lineGeneration = login + "|" + passwordHash + "|"
			+ name + "|\n";
		fileStream << lineGeneration;
	}	

	fileStream.close();

	loginUser_ = login;
	sendData_ = serviceMsg + std::string("You are is registred and login...\n");
	dataTransmission();
		
	return true;
}

bool Chat::loginUser()
{
	std::string login{};
	std::string passwordHash{};

	std::cout << "Login user...\n";

	bool correctUser = false;
	do
	{
		sendData_ = "Login: ";
		dataTransmission();
		dataRecieving();
		login = std::string(message);

		sendData_ = passwordMsg + std::string("Password: ");
		dataTransmission();
		dataRecieving();		
		passwordHash = std::string(message);

		for (auto& element : chatUsers_)
		{
			if (element.getLogin() == login
				&& element.getPasswordHash() == passwordHash)
			{
				correctUser = true;
				loginUser_ = login;
				sendData_ = serviceMsg + std::string("Login user : ");				
				dataTransmission();
				sendData_ = loginMsg + loginUser_;
				dataTransmission();
				break;
			}
		}
		try
		{
			if (!correctUser)
			{
				throw Warning();
			}
		}
		catch (std::exception& warning)
		{
			sendData_ = serviceMsg + std::string(warning.what())
				+ std::string("login or password incorrect.\n");
			std::cout << sendData_;
			dataTransmission();	
			sendData_ = "Type(e) for exit or any key for try again...\n";
			dataTransmission();
			dataRecieving();
						
			if (message[0] == 'e')
			{
				sendData_ = exitChatMsg;
				dataTransmission();
				return false;
			}
		}
	} while (!correctUser);

	return true;
}

void Chat::sendChatData()
{
	if (!chatUsers_.empty())
	{
		std::cout << "Send user list...\n";
		sendUserList();
	}
	if (!chatMessages_.empty())
	{
		std::cout << "Send chat messages...\n";
		sendMessages();
	}	
}

void Chat::sendUserList()
{
	sendData_ = beginUserListMsg;
	dataTransmission();
	
	for (auto& element : chatUsers_)
	{
		sendData_ = element.getName();		
		dataTransmission();

		dataRecieving();
		if (message == "#OK")
		{
			continue;
		}
	}

	sendData_ = endUserListMsg;
	dataTransmission();
}

void Chat::sendMessages()
{
	sendData_ = beginChatListMsg;
	dataTransmission();	
	
	for (auto& element : chatMessages_)
	{
		if (element.getFrom() == loginUser_
			|| element.getTo() == "to_all"
			|| element.getTo() == loginUser_
			|| !element.getIsPrivateMessage())
		{
			sendData_ = element.getFrom() + "|" + element.getTo()
				+ "|" + element.getMessage() + "|";
			dataTransmission();
			dataRecieving();
			if (message == "#OK")
			{
				continue;
			}
		}		

	}	

	sendData_ = endChatListMsg;
	dataTransmission();		
}

bool Chat::chatMenu()
{
	sendData_ = serviceMsg + std::string("User ")
		+ loginUser_ + std::string(" is login...\n");	
	dataTransmission();

	do
	{
		std::cout << "Main menu...\n";
		sendData_ = std::string("Select action:\nn - New message\n")
			+ std::string("v - View messages\nu - User list\n")
			+ std::string("l - Logout(exit client)\nother - Stop chat server\n");		
		dataTransmission();
		dataRecieving();

		switch (message[0])
		{
		case 'n':
			newMessage();
			break;
		case 'v':
			std::cout << "User is viewed messages...\n";			
			break;
		case 'u':
			std::cout << "User is viewed userlist...\n";			
			break;
		case 'l':
			sendData_ = exitChatMsg;
			dataTransmission();			
			close(connection);
			return 1;			
		default:
			sendData_ = exitChatMsg;
			dataTransmission();
			close(sockert_file_descriptor);			
			return 0;				
		}
	} while (true);
}

void Chat::newMessage()
{
//#if defined(_WIN32)
//	system("CLS");
//#else
//	system("clear");
//#endif	
	
	std::string to{ "to_all" };
	bool isPrivateMessage{ false };	
	std::cout << "New messages...\n";
	sendData_ = std::string("Select message type: \nw - write to the user\n")
		+ std::string("p - write to the user privately\nother key - to all\n");
	dataTransmission();	
	dataRecieving();
	
	if (message[0] == 'p')
	{
		isPrivateMessage = true;
	}

	if (message[0] == 'p' || message[0] == 'w')
	{
		sendData_ = "Input user name: ";
		dataTransmission();
		dataRecieving();

		to = std::string(message);

		if (!checkUserName(to))
		{
			sendData_ = serviceMsg + std::string("User not found!\n");
			dataTransmission(); 
			return;
		}
	}
	
	sendData_ = "Input massage text:\n";
	dataTransmission();
	dataRecieving();

	std::string messageText = std::string(message);	
	if (messageText == "")
	{
		return;
	}

	chatMessages_.push_back(Message{ loginUser_, to, messageText,
		isPrivateMessage });
	std::string lineGeneration = loginUser_ + "|" + to + "|"
		+ messageText + "|";
	sendData_ = newMessageMsg + lineGeneration;
	dataTransmission();

	std::ofstream fileStream(messagesFile_, std::ios::app);	

	if (!fileStream)
	{
		std::ofstream fileStream(messagesFile_, std::ios::out | std::ios::app);
		std::filesystem::permissions(messagesFile_,
			std::filesystem::perms::group_all | std::filesystem::perms::others_all,
			std::filesystem::perm_options::remove);
	}
	else	
	{
		lineGeneration += (std::to_string(isPrivateMessage) + "|\n");
		fileStream << lineGeneration;
	}

	fileStream.close();
}

bool Chat::checkUserLogin(std::string& login)
{
	for (auto& element : chatUsers_)
	{
		if (element.getLogin() == login)
		{
			return true;
		}
	}
	return false;
}

bool Chat::checkUserName(std::string& name)
{
	for (auto& element : chatUsers_)
	{
		if (element.getName() == name)
		{
			return true;
		}
	}
	return false;
}

void Chat::readDataFile(std::string& file)
{
	std::string readLine;
	
	std::fstream fileStream(file, std::ios::in);

	if (!fileStream)
	{
		std::cout << "file " << file << " not found...\n!";
		std::fstream fileStream(file, std::ios::out | std::ios::app);
		std::filesystem::permissions(file,
			std::filesystem::perms::group_all | std::filesystem::perms::others_all,
			std::filesystem::perm_options::remove);
	}
	else
	{
		while (std::getline(fileStream, readLine))
		{			
			std::cout << file << " read: " << readLine << "\n";
			if (file == usersFile_)
			{
				readUsersFile(readLine);
			}
			else
			{
				readMessagesFile(readLine);
			}			
		}
	}

	fileStream.close();
}

void Chat::readUsersFile(std::string& line)
{	
	std::string login = dataParsing(line);
	std::string passwordHash = dataParsing(line);
	std::string name = dataParsing(line);

	chatUsers_.push_back(User{ login, passwordHash, name });			
}

void Chat::readMessagesFile(std::string& line)
{
	std::string from = dataParsing(line);
	std::string to = dataParsing(line);
	std::string message = dataParsing(line);
	bool isPrivateMessage = (dataParsing(line) != "0");

	chatMessages_.push_back(Message{ from, to, message, isPrivateMessage });
}

void Chat::dataTransmission()
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

void Chat::dataRecieving()
{
	bzero(message, MESSAGE_LENGTH);
	read(connection, message, sizeof(message));
	std::cout << "The data is received from the client: " << message << "\n";
}

std::string Chat::dataParsing(std::string& data)
{
	std::string separator{ "|" };
	size_t separatorPoint{ data.find(separator) };
	if (separatorPoint == std::string::npos)
	{
		return "Data error...\n";
	}
	std::string value{ data.substr(0, separatorPoint) };
	data.erase(0, separatorPoint + 1);
	return value;
}
