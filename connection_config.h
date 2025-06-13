#pragma once

#include <string>

#define PORT 7777 // Default port 

constexpr const char* connectionConfigFile = "connection.config";

class ConnectionConfig
{
public:
	ConnectionConfig();
	~ConnectionConfig() {}

	const std::string getAddressIP() const;
	const int getPort() const;

private:	
	std::string addressIP_{ "0.0.0.0" };
	int port_{ 0 };

	void readConnectionConfig();
	void checkServerPort();
	void checkServerIPAddress();
	bool parsingIPAddress();
	void setServerPort();
	void setServerIPAddress();
};
