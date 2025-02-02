#include <iostream>
#include <conio.h>
//#include <memory>

#include "chat_class.h"


int main()
{
	system("CLS");
	std::cout << "Welcome to the chat!\n";
	Chat newChat();

	std::cout << "Select action:\nl - Login\nr - Register\ne - Exit programm\n";
	char action{ '\0' };
	do
	{
		action = _getche();
		switch (action)
		{
		case 'l':
			chat.loginUser();
			break;
		case 'r':
			chat.registerUser();
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
