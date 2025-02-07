#include <iostream>
#include <conio.h>
//#include <memory>

#include "chat_class.h"


int main()
{
	system("CLS");
	std::cout << "Welcome to the chat!\n";
	Chat newChat;
	
	char action{ '\0' };
	do
	{
		std::cout << "Select action:\nl - Login\nr - Register\ne - Exit programm\n";
		action = _getche();
		std::cout << "\033[1K\033[0G";
		switch (action)
		{
		case 'l':
			newChat.chatMenu();
			break;
		case 'r':
			newChat.registerUser();
			break;
		case 'e':
			std::cout << "Exit programm...";
			break;
		default:
			std::cout << "Wrong command. Please type l, r or e...";
		}
	} while (action != 'e');

	return 0;
}
