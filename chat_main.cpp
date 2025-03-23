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

	newChat.mainMenu();	
	
	return 0;
}
