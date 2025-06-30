#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>

#include "chat.h"
#include "exception.h"

Chat::Chat()
{   	
	
}

Chat::~Chat()
{
	
}

void Chat::runChat()
{
	if (!server.getPort())
	{
		return;
	}

	readDataFile(usersFile_);
	readDataFile(messagesFile_);

	std::cout << "Run chat...\n";

	while (1)
	{
		server.createConnection();
		
		// Call to register or login
		if (!userAutorization())
		{
			std::cout << "End chat...\n";
			server.stopServer();			
			return;
		}

		sendChatData();

		if (!chatMenu())
		{
			std::cout << "End chat...\n";
			server.stopServer();			
			return;
		}
		else
		{
			std::cout << "Logout...\n";
			close(server.connection);
		}
	}	
}

bool Chat::userAutorization()
{
	std::cout << "Register...\n";
	server.sendData_ = serverCommand::helloMsg
		+ textColor::green
		+ "Hello! Please login(l), register (r) or exit(other key)...\n"
		+ textColor::resetColor;
	server.dataTransmission();
	server.dataRecieving();
		
	switch (server.message[0])
	{
	case 'l':
		if (chatUsers_.empty())
		{
			server.sendData_ = serverCommand::serviceMsg
				+ textColor::red
				+ "Users not found. Please register...\n"
				+ textColor::resetColor;
			server.dataTransmission();
			return registerUser();
		}
		else
		{
			return loginUser();
		}		
	case 'r':
		return registerUser();		
	default:
		server.sendData_ = serverCommand::exitChatMsg;
		server.dataTransmission();
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
		server.sendData_ = textColor::yellow
			+ "Login: \0"
			+ textColor::resetColor;
		server.dataTransmission();
		server.dataRecieving();
		login = std::string(server.message);

		try
		{
			if (checkUserLogin(login))
			{
				throw Warning();
			}
		}
		catch (std::exception& warning)
		{			
			server.sendData_ = serverCommand::serviceMsg
				+ textColor::red
				+ std::string(warning.what())
				+ "login is busy. Ñhoose a different login...\n"
				+ textColor::resetColor;
			std::cout << server.sendData_;
			server.dataTransmission();
			valueIsBusy = true;
			break;
		}	
	} while (valueIsBusy);
	
	server.sendData_ = serverCommand::passwordMsg
		+ textColor::yellow
		+ "Password: "
		+ textColor::resetColor;
	server.dataTransmission();
	server.dataRecieving();
	passwordHash = std::string(server.message);

	do
	{
		valueIsBusy = false;
		server.sendData_ = textColor::yellow
			+ "Name: "
			+ textColor::resetColor;
		server.dataTransmission();
		server.dataRecieving();
		name = std::string(server.message);
		try
		{
			if (checkUserName(name))
			{
				throw Warning();
			}
		}
		catch (std::exception& warning)
		{
			server.sendData_ = serverCommand::serviceMsg
				+ textColor::red
				+ std::string(warning.what())
				+ "name is busy. Ñhoose a different name...\n"
				+ textColor::resetColor;
			std::cout << server.sendData_;
			server.dataTransmission();
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
	server.sendData_ = serverCommand::serviceMsg
		+ textColor::green
		+ "You are is registred and login...\n"
		+ textColor::resetColor;
	server.dataTransmission();
		
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
		server.sendData_ = textColor::yellow
			+ "Login: "
			+ textColor::resetColor;
		server.dataTransmission();
		server.dataRecieving();
		login = std::string(server.message);

		server.sendData_ = serverCommand::passwordMsg
			+ textColor::yellow
			+ "Password: "
			+ textColor::resetColor;
		server.dataTransmission();
		server.dataRecieving();
		passwordHash = std::string(server.message);

		for (auto& element : chatUsers_)
		{
			if (element.getLogin() == login
				&& element.getPasswordHash() == passwordHash)
			{
				correctUser = true;
				loginUser_ = login;				
				server.sendData_ = serverCommand::loginMsg + loginUser_;
				server.dataTransmission();
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
			server.sendData_ = textColor::red
				+ std::string(warning.what())
				+ "login or password incorrect. "
				+ "Type(e) for exit or any key for try again...\n"
				+ textColor::resetColor;
			server.dataTransmission();
			server.dataRecieving();
						
			if (server.message[0] == 'e')
			{
				server.sendData_ = serverCommand::exitChatMsg;
				server.dataTransmission();
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
	server.sendData_ = serverCommand::beginUserListMsg;
	server.dataTransmission();
	
	for (auto& element : chatUsers_)
	{
		server.sendData_ = element.getName();
		server.dataTransmission();

		server.dataRecieving();
		if (server.message == serverCommand::confirmationMsg)
		{
			continue;
		}
	}

	server.sendData_ = serverCommand::endUserListMsg;
	server.dataTransmission();
}

void Chat::sendMessages()
{
	server.sendData_ = serverCommand::beginChatListMsg;
	server.dataTransmission();
	
	for (auto& element : chatMessages_)
	{
		if (element.getFrom() == loginUser_
			|| element.getTo() == "to_all"
			|| element.getTo() == loginUser_)			
		{
			server.sendData_ = element.getFrom() + "|" + element.getTo()
				+ "|" + element.getMessage() + "|";
			server.dataTransmission();
			server.dataRecieving();
			if (server.message == serverCommand::confirmationMsg)
			{
				continue;
			}
		}	
	}	

	server.sendData_ = serverCommand::endChatListMsg;
	server.dataTransmission();
}

bool Chat::chatMenu()
{
	server.sendData_ = serverCommand::serviceMsg
		+ textColor::green
		+ "\nUser "
		+ loginUser_
		+ " is login...\n"
		+ textColor::resetColor;
	server.dataTransmission();

	do
	{
		std::cout << "Main menu...\n";
		server.sendData_ = textColor::cyan
			+ "\nSelect action:\nn - New message\nv - View messages\nu - User "
			+ "list\nl - Logout(exit client)\nother - Stop chat server\n"
			+ textColor::resetColor;
		server.dataTransmission();
		server.dataRecieving();

		switch (server.message[0])
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
			server.sendData_ = serverCommand::exitChatMsg;
			server.dataTransmission();
			close(server.connection);
			return 1;			
		default:
			server.sendData_ = serverCommand::exitChatMsg;
			server.dataTransmission();
			server.stopServer();
			return 0;				
		}
	} while (true);
}

void Chat::newMessage()
{
	std::string to{ "to_all" };	
	std::cout << "\nNew messages...\n";
	server.sendData_ = textColor::cyan
		+ "Select message type: \nw - write to the user\nother key - to all\n"
		+ textColor::resetColor;
	server.dataTransmission();
	server.dataRecieving();	
	
	if (server.message[0] == 'w')
	{
		server.sendData_ = textColor::yellow
			+ "Input user name: "
			+ textColor::resetColor;
		server.dataTransmission();
		server.dataRecieving();

		to = std::string(server.message);

		if (!checkUserName(to))
		{
			server.sendData_ = serverCommand::serviceMsg
				+ textColor::red
				+ "User not found!\n"
				+ textColor::resetColor;
			server.dataTransmission();
			return;
		}
	}

	server.sendData_ = textColor::yellow
		+ "Input message text:\n"
		+ textColor::resetColor;
	server.dataTransmission();
	server.dataRecieving();

	std::string messageText = std::string(server.message);
	if (messageText.empty())
	{
		server.sendData_ = serverCommand::serviceMsg
			+ textColor::red
		    + "The message is empty...\n"
			+ textColor::resetColor;
		server.dataTransmission();
		return;
	}

	chatMessages_.push_back(Message{ loginUser_, to, messageText });
	std::string lineGeneration = loginUser_ + "|" + to + "|"
		+ messageText + "|";
	server.sendData_ = serverCommand::newMessageMsg + lineGeneration;
	server.dataTransmission();

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
		lineGeneration += "\n";
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

	chatMessages_.push_back(Message{ from, to, message });
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
