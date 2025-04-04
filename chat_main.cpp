#include <iostream>

#include "chat_class.h"

#if defined(_WIN32)
#include <process.h>
#include <windows.h>
#else
#include <unistd.h>
#include <sys/utsname.h>
#endif

int main()
{	
#if defined(_WIN32)
	system("CLS");
	std::cout << "PID: " << _getpid() << std::endl;
	std::printf("PID: %Xh", GetCurrentProcessId());
	system("ver");
	std::cout << std::endl;
#else	
	struct utsname utsname;

	uname(&utsname);
	system("clear");

	cout << "PID: " << getpid() << std::endl;
	cout << "OS name: " << utsname.sysname << std::endl;
	cout << "OS release: " << utsname.release << std::endl;
	cout << "OS version: " << utsname.version << std::endl;	
#endif	

	std::cout << "Welcome to the chat!\n";
	Chat newChat;
	
	newChat.mainMenu();

	return 0;
}
