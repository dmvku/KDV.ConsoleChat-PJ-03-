#include <iostream>

#include "connection_config.h"
#include "chat.h"
#include "socket.h"

int main()
{    
    Chat chat;
    chat.runChat();    
    
    std::cout << "Server is stopped...\nExit chat...\n";

    return 0;
}
