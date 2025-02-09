#include "console.h"

void gotoCoordinates(int x, int y)         // Changing cursor coordinates
{
	HANDLE handleConsole;
	COORD cursorLocate;
	std::cout.flush();
	cursorLocate.X = x;
	cursorLocate.Y = y;
	handleConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(handleConsole, cursorLocate);
}

int getXcoord()                  // Getting the X coordinate of the cursor
{
	CONSOLE_SCREEN_BUFFER_INFO info_x;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info_x);
	return info_x.dwCursorPosition.X;
}

int getYcoord()                  // Getting the Y coordinate of the cursor
{
	CONSOLE_SCREEN_BUFFER_INFO info_y; 
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info_y);
	return info_y.dwCursorPosition.Y;
}

void clearingTheInputWindow()
{
	gotoCoordinates(0, 23);
	std::cout << "\033[A\033[2K\033[A\033[2K\033[A\033[2K\033[A\033[2K";
}

void outInformationLines()       // Displays the fields of the input window
{
	gotoCoordinates(0, 18);
	std::cout << serviceLine_;
	gotoCoordinates(0, 24);
	std::cout << serviceLine_;
}
void outSelectAction()           // Outputs the title
{
	gotoCoordinates(32, 18);
	std::cout << "\033[1;33;44mSelect action:\033[0m";
}
