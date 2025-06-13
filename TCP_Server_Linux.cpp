#include <iostream>

#include "connection_config.h"
#include "chat_class.h"

int main()
{
    int port{ readConnectionConfig() };

    if (port)
    {
        Chat server(port);
        server.runChat();
    }
    
    std::cout << "Server is stoping...\nExit chat...\n";

    return 0;
}
