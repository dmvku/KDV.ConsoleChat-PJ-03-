#include "server_config.h"

#include <iostream>
#include <string>
#include <fstream>
#include <climits>
#include <filesystem>

ServerConfig::ServerConfig()
{
	fileOptions();
}

ServerConfig::~ServerConfig()
{

}

void ServerConfig::fileOptions()
{
	openFile();
	readFile();
	fileStream.close();
	checkParameters();
	saveFile();
}

void ServerConfig::openFile()
{
	fileStream.open(serverConfigFile, std::ios::in);
	if (!fileStream.is_open())
	{
		std::cout << "Server config file not found...\n";
		createFile();
		setPort();
	}
}

void ServerConfig::readFile()
{
	std::string dataLine;
	std::string separator{ ":" };

	while (std::getline(fileStream, dataLine))
	{
		size_t separatorPoint = dataLine.find(separator);
		if (separatorPoint != std::string::npos)
		{
			std::string parameterName = dataLine.substr(0, separatorPoint);
			if (parameterName == parametersName::s_port)
			{
				port_ = stoi(dataLine.substr(separatorPoint + 1));
				return;
			}
		}
		
		std::cout << "Server parameter not found or corrupted...\n";
		setPort();
	}
}

void ServerConfig::checkParameters()
{
	std::cout << "Actual server parameters:\n"		
		<< parametersName::s_port << ": " << port_ << "\n";

	std::cout << "Do you want to change parameters (y/n)? ";
	char action{ '\0' };
	std::cin >> action;
	if (action == 'y')
	{
		setPort();
	}
}

void ServerConfig::createFile()
{	
	std::fstream fileStream(serverConfigFile,
		std::ios::in | std::ios::out | std::ios::app);
	std::filesystem::permissions(serverConfigFile,
		std::filesystem::perms::group_all | std::filesystem::perms::others_all,
		std::filesystem::perm_options::remove);
}

void ServerConfig::saveFile()
{
	fileStream.open(serverConfigFile, std::ios::out | std::ios::trunc);
	if (!fileStream.is_open())
	{
		createFile();
	}
	fileStream << parametersName::s_port << ":" << port_ << "\n";
	fileStream.close();
}

void ServerConfig::checkPort()
{
	int minPort{ 1024 };

	if (port_ < minPort || port_ > USHRT_MAX)
	{
		if (!port_)
		{
			return;
		}

		std::cout << "Illegal port: " << port_ << "\n";
		port_ = PORT;
		std::cout << "The default port value is set: " << port_ << "\n";		
	}
	return;
}

void ServerConfig::setPort()
{	
	std::cout << "Set valid port from 1024 to 65535 (0 - stop server): ";
	std::cin >> port_;	
	checkPort();
}

const int ServerConfig::getPort() const
{
	return port_;
}
