#include <iostream>
#include <conio.h>
#include <windows.h>
//#include <memory>

#include "chat_class.h"


int main()
{
	// Set windows parameter
	/*HANDLE hWnd = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD bufferSize = { 80, 24 };
	SetConsoleScreenBufferSize(hWnd, bufferSize);
	system("mode con cols=80 lines=25");*/
	system("CLS");

	std::cout << "Welcome to the chat!\n";
	Chat newChat;
	
	char action{ '\0' };
	do
	{
		std::cout << "Select action:\nl - Login\nr - Register\ne - Exit programm\n";
		action = _getche();
		std::cout << "\b";
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
