#include "chat_class.h"
#include "console.h"

#include <iostream>
#include <conio.h>

int main()
{
	// Set windows parameter
	HANDLE hWnd = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD bufferSize = { 80, 24 };
	SetConsoleScreenBufferSize(hWnd, bufferSize);
	system("mode con cols=80 lines=25");
	system("CLS");

	gotoCoordinates(30, 9);
	std::cout << "Welcome to the chat!\n";
	outInformationLines();
	outSelectAction();

	Chat newChat;
	
	char action{};
	do
	{			
		gotoCoordinates(27, 24);
		std::cout << "\033[1;36;44ml\033[33mogin    \033[36mr\033[33megister    \033[36me\033[33mxit\033[0m";
		gotoCoordinates(0, 19);
		
		action = _getche();
		std::cout << "\033[2K";
		switch (action)
		{
		case 'l':
			newChat.chatMenu();
			break;
		case 'r':
			newChat.registerUser();
			outSelectAction();
			break;
		case 'e':
			system("CLS");
			std::cout << "Exit programm...";
			break;
		default:
			system("CLS");
			outInformationLines();
			gotoCoordinates(20, 18);
			std::cout << "\033[1;33;44mWrong command. Please type l, r or e...\033[0m";
		}
	} while (action != 'e');

	return 0;
}
