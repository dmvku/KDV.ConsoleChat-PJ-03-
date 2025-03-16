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

	outInformationLines();
	gotoCoordinates(29, 18);
	std::cout << "\033[1;33;44mWelcome to the chat!\033[0m";

	Chat newChat;
	
	char action{};
	do
	{			
		gotoCoordinates(18, 24);
		std::cout << "\033[1;36;44ml\033[33mogin    \033[36mr\033[33megister user    "
			<< "\033[36md\033[33melete user   \033[36me\033[33mxit\033[0m";
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
			//outSelectAction();
			break;
		case 'd':
			newChat.deleteUser();
			//outSelectAction();
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
