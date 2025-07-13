#include "DB_config.h"

//#include <unistd.h>
#include <iostream>
#include <fstream>
#include <climits>
#include <filesystem>

DBConfig::DBConfig()
{
	fileOptions();
}

DBConfig::~DBConfig()
{

}

void DBConfig::fileOptions()
{
	openFile();
	readFile();	
	fileStream.close();
	checkParameters();
	saveFile();
}

void DBConfig::openFile()
{
	fileStream.open(DBConfigFile, std::ios::in);
	if (!fileStream.is_open())
	{
		std::cout << "Database config file not found...\n";
		createFile();
		setDBConnectionParameters();
	}	
}

void DBConfig::readFile()
{
	std::string dataLine;
	std::string separator{ ":" };

	while (std::getline(fileStream, dataLine))
	{
		size_t separatorPoint = dataLine.find(separator);
		if (separatorPoint == std::string::npos)
		{
			std::cout << "Database parameter not found or corrupted...\n";
			setDBConnectionParameters();			
		}
		else
		{			
			std::string parameterName = dataLine.substr(0, separatorPoint);
			if (parameterName == parametersName::n_host)
			{
				hostDB_ = dataLine.substr(separatorPoint + 1);
			}
			else if (parameterName == parametersName::n_user)
			{
				userDB_ = dataLine.substr(separatorPoint + 1);
			}
			else if (parameterName == parametersName::n_password)
			{
				passwordDB_ = dataLine.substr(separatorPoint + 1);
			}
			else if (parameterName == parametersName::n_database)
			{
				nameDB_ = dataLine.substr(separatorPoint + 1);
			}
			else if (parameterName == parametersName::n_port)
			{
				portDB_ = stoi(dataLine.substr(separatorPoint + 1));
			}
		}
	}
}

void DBConfig::checkParameters()
{
	checkHostDB();
	checkPortDB();
	std::cout << "Actual databsase parameters:\n"
			<< parametersName::n_host << ": " << hostDB_ << "\n"
			<< parametersName::n_user << ": " << userDB_ << "\n"
	    	<< parametersName::n_password << ": " << passwordDB_ << "\n"
			<< parametersName::n_database << ": " << nameDB_ << "\n"
			<< parametersName::n_port << ": " << portDB_ << "\n";						
	
	std::cout << "Do you want to change database parameters (y/n)? ";
	char action{ '\0' };
	std::cin >> action;
	if (action == 'y')
	{
		setDBConnectionParameters();
	}		
}

void DBConfig::createFile()
{	
	std::fstream fileStream(DBConfigFile,
		std::ios::in | std::ios::out | std::ios::app);
	std::filesystem::permissions(DBConfigFile,
		std::filesystem::perms::group_all | std::filesystem::perms::others_all,
		std::filesystem::perm_options::remove);
}
	
void DBConfig::saveFile()
{
	fileStream.open(DBConfigFile, std::ios::out | std::ios::trunc);
	if (!fileStream.is_open())
	{
		createFile();
	}
	fileStream << parametersName::n_host << ":" << hostDB_ << "\n"
		<< parametersName::n_user << ":" << userDB_ << "\n"
		<< parametersName::n_password << ":" << passwordDB_ << "\n"
		<< parametersName::n_database << ":" << nameDB_ << "\n"
		<< parametersName::n_port << ":" << portDB_ << "\n";	
	fileStream.close();
}

void DBConfig::setDBConnectionParameters()
{
	setHostDB();
	setUserDB();
	setPasswordDB();
	setNameDB();
	setPortDB();	
}

void DBConfig::checkPortDB()
{	
	if (portDB_ < 0 || portDB_ > USHRT_MAX)
	{		
		std::cout << "Illegal port: " << portDB_ << "\n";		
		portDB_ = DBPORT;
		std::cout << "The default port value is set: " << portDB_ << "\n";		
	}
	return;
}

void DBConfig::checkHostDB()
{
	if (hostDB_ == "localhost")
	{
		return;
	}

	if (!parsingAddressIP())
	{
		std::cout << "Illegal IP address: " << hostDB_ << "\n";
		hostDB_ = "TCP://127.0.0.1";
		std::cout << "The default IP address value is set: "
			<< hostDB_ << "\n";
	}
	else
	{
		hostDB_.insert(0, "TCP://");
	}

	return;
}

bool DBConfig::parsingAddressIP()
{
	std::string separator{ "." };
	int numberOfOctets{ 4 };
	int octet{};	
	std::string address = hostDB_;

	for (int octetsCount{ 1 }; octetsCount <= numberOfOctets; octetsCount++)
	{
		if (octetsCount < numberOfOctets)
		{
			size_t separatorPoint = address.find(separator);
			if (separatorPoint == 0 || separatorPoint == std::string::npos)
			{
				std::cout << "Invalid IP address: " << hostDB_ << "\n";
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
			std::cout << "Invalid IP address: " << hostDB_ << "\n";
			return false;
		}
	}
	return true;
}

void DBConfig::setHostDB()
{
	std::cout << "Set the DB server IP address in the format XXX.XXX.XXX.XXX "
		<< "or 'localhost': ";
	std::cin >> hostDB_;
	checkHostDB();
}
void DBConfig::setUserDB()
{
	std::cout << "Input database user name: ";
	std::cin >> userDB_;
}

void DBConfig::setPasswordDB()
{	
	std::cout << "Input password: ";
	std::cin >> passwordDB_;	
}

void DBConfig::setNameDB()
{
	std::cout << "Input database name for connection: ";
	std::cin >> nameDB_;
}

void DBConfig::setPortDB()
{
	std::cout << "Set valid port from 0 to 65535 (default - 3306): ";
	std::cin >> portDB_;
	checkPortDB();
}

const std::string DBConfig::getHostDB() const
{
	return hostDB_;
}

const std::string DBConfig::getUserDB() const
{
	return userDB_;
}

const std::string DBConfig::getPasswordDB() const
{
	return passwordDB_;
}

const std::string DBConfig::getNameDB() const
{
	return nameDB_;
}

const int DBConfig::getPortDB() const
{
	return portDB_;
}
