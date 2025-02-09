#pragma once
#include <iostream>
#include <windows.h>

// Menu display
const std::string serviceLine_
{
	"\033[44m                                                                                \033[0m"
};
const std::string chatMenu_{ "\033[1;33mSelect action:\033[0m" };

void gotoCoordinates(int x, int y);
int getXcoord();
int getYcoord();
void clearingTheInputWindow();
void outInformationLines();
void outSelectAction();
