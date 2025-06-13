#include "connection_config.h"

#include <iostream>
#include <string>
#include <fstream>
#include <climits>
#include <filesystem>

int readConnectionConfig()
{
	std::string dataLine;
	std::string keyWord{ "ServerPort" };
	std::string separator{ ":" };
	int port{ 0 };

	std::fstream fileStream(connectionConfigFile, std::ios::in);
	
	if (!fileStream)
	{
		std::cout << "Server parameter file not found...\n";
		std::fstream fileStream(connectionConfigFile, std::ios::in | std::ios::out | std::ios::app);
		std::filesystem::permissions(connectionConfigFile,
			std::filesystem::perms::group_all | std::filesystem::perms::others_all,
			std::filesystem::perm_options::remove);
	}
	else
	{
		std::getline(fileStream, dataLine);
		size_t separatorPoint = dataLine.find(separator);
		if (separatorPoint == std::string::npos
			|| dataLine.substr(0, separatorPoint) != keyWord)
		{
			std::cout << "Server parameter not found...\n";			
		}
		else
		{
			dataLine.erase(0, separatorPoint + 1);
			port = std::stoi(dataLine.substr(0));
			std::cout << "Find server parameters: Port:" << port << "\n";
			std::cout << "Do you want to change the port (y/n)? ";
			char action{ '\0' };
			std::cin >> action;
			if (action == 'y')
			{
				port = setServerParameters();
			}
		}
	}
	
	fileStream.close();

	return checkServerParameters(port);	
}

int checkServerParameters(int port)
{
	int minPort{ 1024 };

	if (port < minPort || port > USHRT_MAX)
	{
		if (!port)
		{
			return port;
		}

		std::cout << "Illegal port: " << port << "\n";
		port = setServerParameters();
		
		if (port < minPort || port > USHRT_MAX)
		{
			port = PORT;
			std::cout << "The default port value is set: " << port << "\n";
		}
		std::fstream fileStream(connectionConfigFile, std::ios::app | std::ios::trunc);

		if (fileStream.is_open())
		{
			fileStream << "ServerPort:" << std::to_string(port);
		}
		fileStream.close();		
	}
	return port;
}

int setServerParameters()
{	
	int port{ 0 };
	std::cout << "Set valid port from 1024 to 65535 (0 - stop server): ";
	std::cin >> port;
	return port;
}
