#include "connection_config.h"

#include <iostream>
#include <fstream>
#include <climits>
#include <filesystem>

ConnectionConfig::ConnectionConfig()
{
	readConnectionConfig();
}

void ConnectionConfig::readConnectionConfig()
{
	std::string dataLine;
	std::string keyWord{ "Server" };
	std::string separator{ ":" };

	std::fstream fileStream(connectionConfigFile, std::ios::in);
	
	if (!fileStream.is_open())
	{
		std::cout << "Server parameter file not found...\n";
		std::fstream fileStream(connectionConfigFile, std::ios::in | std::ios::out | std::ios::app);
		std::filesystem::permissions(connectionConfigFile,
			std::filesystem::perms::group_all | std::filesystem::perms::others_all,
			std::filesystem::perm_options::remove);
		setServerPort();
		setServerIPAddress();
	}
	else
	{		
		std::getline(fileStream, dataLine);
		size_t separatorPoint = dataLine.find(separator);
		if (separatorPoint == std::string::npos
			|| dataLine.substr(0, separatorPoint) != keyWord)
		{
			std::cout << "Server parameter not found...\n";	
			setServerPort();
			setServerIPAddress();
		}
		else
		{
			dataLine.erase(0, separatorPoint + 1);
			size_t separatorPoint = dataLine.find(separator);
			addressIP_ = dataLine.substr(0, separatorPoint);
			port_ = std::stoi(dataLine.substr(separatorPoint + 1));
			std::cout << "Find server parameters: " << addressIP_
				<< ":" << port_ << "\n";
			std::cout << "Do you want to change server parameters (y/n)? ";
			char action{ '\0' };
			std::cin >> action;
			if (action == 'y')
			{				
				setServerPort();
				setServerIPAddress();
			}
		}
	}

	fileStream.close();

	checkServerPort();
	checkServerIPAddress();

	std::fstream fs(connectionConfigFile,
		std::ios::out | std::ios::trunc);
	if (fs.is_open())
	{
		fs << keyWord << separator << addressIP_ << ":" << port_;
	}
	fs.close();
}

void ConnectionConfig::checkServerPort()
{
	int minPort{ 1024 };

	if (port_ < minPort || port_ > USHRT_MAX)
	{
		if (!port_)
		{
			return;
		}

		std::cout << "Illegal port: " << port_ << "\n";
		setServerPort();

		if (port_ < minPort || port_ > USHRT_MAX)
		{
			port_ = PORT;
			std::cout << "The default port value is set: " << port_ << "\n";
		}
	}
	
	return;
}

void ConnectionConfig::checkServerIPAddress()
{		
	if (addressIP_ == "0")
	{
		return;
	}		
	
	if (!parsingIPAddress())
	{
		setServerIPAddress();
	}

	if (!parsingIPAddress())
	{
		addressIP_ = "0";
	}

	return;
}

bool ConnectionConfig::parsingIPAddress()
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

void ConnectionConfig::setServerPort()
{	
	std::cout << "Set valid port from 1024 to 65535 (0 - stop server): ";
	std::cin >> port_;	
}

void ConnectionConfig::setServerIPAddress()
{
	std::cout << "Set the IP address in the format XXX.XXX.XXX.XXX\n";
	std::cin >> addressIP_;	
}

const std::string ConnectionConfig::getAddressIP() const
{
	return addressIP_;
}

const int ConnectionConfig::getPort() const
{
	return port_;
}
