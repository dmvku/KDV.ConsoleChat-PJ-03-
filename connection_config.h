#pragma once

#include <string>

#define PORT 7777 // Default port 

constexpr const char* connectionConfigFile = "connection.config";

// int port_{};

int readConnectionConfig();
int checkServerParameters(int port);
int setServerParameters();
