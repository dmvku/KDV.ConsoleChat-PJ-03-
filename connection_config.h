#pragma once

#include <string>
#include <fstream>

#define PORT 7777 // Default port 

extern const char* connectionConfigFile;

namespace parametersName
{
	const std::string n_addressIP = "IP address";	
	const std::string n_port = "Port";
}

class ConnectionConfig
{
public:
	ConnectionConfig();
	~ConnectionConfig();

	const std::string getAddressIP() const;
	const int getPort() const;

private:
	std::fstream fileStream;
	std::string addressIP_{ "127.0.0.1" };
	unsigned int port_{ PORT };

	void fileOptions();
	void openFile();
	void readFile();
	void checkParameters();
	void createFile();
	void saveFile();

	void checkPort();
	void checkAddressIP();
	bool parsingAddressIP();

	void setConnectionParameters();
	void setAddressIP();	
	void setPort();
};
