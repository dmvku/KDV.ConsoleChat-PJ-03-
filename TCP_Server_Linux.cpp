#include <iostream>

#include "server_config.h"
#include "chat.h"
#include "socket.h"

extern const char* DBConfigFile = "database.config";
extern const char* serverConfigFile = "connection.config";
extern const char* serverLogFile = "server.log";

int main()
{    
    Chat chat;
    chat.runChat();    
    
    std::cout << "Server is stopped...\nExit chat...\n";

    return 0;
}
