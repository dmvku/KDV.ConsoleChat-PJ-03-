#include <iostream>

#include "chat_class.h"

#if defined(__linux__)
#include <unistd.h>
#include <sys/utsname.h>
#else
#include <process.h>
#include <windows.h>
#endif

int main()
{	
#if defined(__linux__)
    struct utsname utsname;

    uname(&utsname); 
	system("clear");

	cout << "PID: " << getpid() << std::endl;
    cout << "OS name: " << utsname.sysname << std::endl;
    cout << "OS release: " << utsname.release << std::endl;
    cout << "OS version: " << utsname.version << std::endl;
#else
	
	system("CLS");
	std::cout << "PID: " << _getpid() << std::endl;
	std::printf("PID: %Xh", GetCurrentProcessId());
	system("ver");
	std::cout << std::endl;
#endif	

	std::cout << "Welcome to the chat!\n";
	Chat newChat;
	
	newChat.mainMenu();

	return 0;
}
