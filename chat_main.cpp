#include <iostream>
#include <conio.h>
//#include <memory>


int main()
{
	system("CLS");
	std::cout << "Welcome to the chat!\n";

	std::cout << "Select action:\nl - Login\nr - Register\ne - Exit programm\n";
	char action{ '\0' };
	do
	{
		action = _getche();
		switch (action)
		{
		case 'l':
			loginUser();
			break;
		case 'r':
			registerUser();
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
