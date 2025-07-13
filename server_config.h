#pragma once

#include <string>
#include <fstream>

#define PORT 7777 // Default port 

extern const char* serverConfigFile;

namespace parametersName
{
	const std::string s_port = "Port";	
}

class ServerConfig
{
public:
	ServerConfig();
	~ServerConfig();
	
	const int getPort() const;

private:
	std::fstream fileStream;	
	unsigned int port_{ 0 };

	void fileOptions();
	void openFile();
	void readFile();
	void checkParameters();
	void createFile();
	void saveFile();

	void checkPort();		
	void setPort();
};
