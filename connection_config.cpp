#include "connection_config.h"

#include <iostream>
#include <fstream>
#include <climits>
#include <filesystem>

ConnectionConfig::ConnectionConfig()
{
	fileOptions();
}

ConnectionConfig::~ConnectionConfig()
{

}

void ConnectionConfig::fileOptions()
{
	openFile();
	readFile();
	fileStream.close();
	checkParameters();
	saveFile();
}

void ConnectionConfig::openFile()
{
	fileStream.open(connectionConfigFile, std::ios::in);
	if (!fileStream.is_open())
	{
		std::cout << "Connection config file not found...\n";
		createFile();
		setConnectionParameters();
	}
}

void ConnectionConfig::readFile()
{
	std::string dataLine;
	std::string separator{ ":" };

	while (std::getline(fileStream, dataLine))
	{
		size_t separatorPoint = dataLine.find(separator);
		if (separatorPoint == std::string::npos)
		{
			std::cout << "Connection parameter not found or corrupted...\n";
			setConnectionParameters();
		}
		else
		{
			std::string parameterName = dataLine.substr(0, separatorPoint);
			if (parameterName == parametersName::n_addressIP)
			{
				addressIP_ = dataLine.substr(separatorPoint + 1);
			}			
			else if (parameterName == parametersName::n_port)
			{
				port_ = stoi(dataLine.substr(separatorPoint + 1));
			}
		}
	}
}

void ConnectionConfig::checkParameters()
{
	checkAddressIP();
	checkPort();
	std::cout << "Actual connection parameters:\n"
		<< parametersName::n_addressIP << ": " << addressIP_ << "\n"		
		<< parametersName::n_port << ": " << port_ << "\n";

	std::cout << "Do you want to change connection parameters (y/n)? ";
	char action{ '\0' };
	std::cin >> action;
	if (action == 'y')
	{
		setConnectionParameters();
	}
}

void ConnectionConfig::createFile()
{
	std::fstream fileStream(connectionConfigFile,
		std::ios::in | std::ios::out | std::ios::app);
	std::filesystem::permissions(connectionConfigFile,
		std::filesystem::perms::group_all | std::filesystem::perms::others_all,
		std::filesystem::perm_options::remove);
}

void ConnectionConfig::saveFile()
{
	fileStream.open(connectionConfigFile, std::ios::out | std::ios::trunc);
	if (!fileStream.is_open())
	{
		createFile();
	}
	fileStream << parametersName::n_addressIP << ":" << addressIP_ << "\n"		
		<< parametersName::n_port << ":" << port_ << "\n";
	fileStream.close();
}

void ConnectionConfig::setConnectionParameters()
{
	setAddressIP();	
	setPort();
}

void ConnectionConfig::checkPort()
{
	int minPort{ 1024 };

	if (port_ < minPort || port_ > USHRT_MAX)
	{
		std::cout << "Illegal port: " << port_ << "\n";
		port_ = PORT;
		std::cout << "The default port value is set: " << port_ << "\n";
	}
	return;
}

void ConnectionConfig::checkAddressIP()
{		
	if (!parsingAddressIP())
	{
		std::cout << "Illegal IP address: " << addressIP_ << "\n";
		addressIP_ == "127.0.0.1";
		std::cout << "The default IP address value is set: "
			<< addressIP_ << "\n";
	}
	return;
}

bool ConnectionConfig::parsingAddressIP()
{
	std::string separator{ "." };
	int numberOfOctets{ 4 };
	int octet{};
	std::string address = addressIP_;

	for (int octetsCount{ 1 }; octetsCount <= numberOfOctets; octetsCount++)
	{
		if (octetsCount < numberOfOctets)
		{
			size_t separatorPoint = address.find(separator);
			if (separatorPoint == 0 || separatorPoint == std::string::npos)
			{
				std::cout << "Invalid IP address: " << addressIP_ << "\n";
				return false;
			}
			octet = std::stoi(address.substr(0, separatorPoint));
			address.erase(0, separatorPoint + 1);
		}
		else
		{
			octet = std::stoi(address.substr(0, 3));
		}

		if (octet < 0 || octet > UCHAR_MAX)
		{
			std::cout << "Invalid IP address: " << addressIP_ << "\n";
			return false;
		}
	}
	return true;
}

void ConnectionConfig::setAddressIP()
{
	std::cout << "Set the DB server IP address in the format"
		<< "XXX.XXX.XXX.XXX (default - 127.0.0.1): ";
	std::cin >> addressIP_;
	checkAddressIP();
}

void ConnectionConfig::setPort()
{
	std::cout << "Set valid port from 1024 to 65535 (default - 7777): ";
	std::cin >> port_;
	checkPort();
}

const std::string ConnectionConfig::getAddressIP() const
{
	return addressIP_;
}

const int ConnectionConfig::getPort() const
{
	return port_;
}
