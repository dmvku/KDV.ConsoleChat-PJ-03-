#include <iostream>

#include "chat_class.h"

int main()
{	
	system("clear");

	std::cout << "Welcome to the chat!\n";
	Chat newChat;
	
	newChat.mainMenu();

	return 0;
}
