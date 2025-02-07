#include "chat_class.h"

using namespace std;
//Chat::Chat()
//{	
//	
//}

Chat::~Chat()
{

}

void Chat::registerUser()
{
	std::string login{};
	std::string password{};
	std::string name{};
	bool valueIsBusy = false;
	do
	{
		std::cout << "What's your name: ";
		std::cin >> name;
		std::cout << "Login: ";
		std::cin >> login;
		for (auto& element : chatUsers_)
		{
			try
			{
				if (element.getLogin() == login)
				{
					throw Warning();
				}
			}
			catch (std::exception& warning)
			{
				std::cout << warning.what() << "login is busy. Ñhoose a different login...\n";
				valueIsBusy = true;
				break;
			}
		}
	} while (valueIsBusy);

	std::cout << "Password: ";
	std::cin >> password;

	valueIsBusy = false;
	do
	{
		std::cout << "Name: ";
		std::cin >> name;

		for (auto& element : chatUsers_)
		{
			try
			{
				if (element.getName() == name)
				{
					throw Warning();
				}
			}
			catch (std::exception& warning)
			{
				std::cout << warning.what() << "name is busy. Ñhoose a different name...\n";
				valueIsBusy = true;
				break;
			}
		}
	} while (valueIsBusy);

	chatUsers_.push_back(User{ login, password, name });
}

void Chat::loginUser()
{
	std::string login{};
	std::string password{};
	
	bool correctUser = false;
	do
	{
		std::cout << "Login: ";
		std::cin >> login;
		std::cout << "Password: ";
		std::cin >> password;

		for (auto& element : chatUsers_)
		{
			if (element.getLogin() == login && element.getPassword() == password)
			{
				correctUser = true;
				loginUser_ = std::make_shared<User>(element);
				break;
			}
		}
		try
		{
			if (!correctUser)
			{
				throw Warning();
			}
		}
		catch (std::exception& warning)
		{
			std::cout << warning.what() << "login or password incorrect. Try again...\n";
		}		
	} while (correctUser);

	while (loginUser_)
	{
		chatMenu();
	};
}

void Chat::chatMenu()
{
	std::cout << "Select action:\nn - New message\nv - View messages\nu - User list\ne - Exit chat\n";
	char action{ '\0' };
	do
	{
		action = _getche();
		switch (action)
		{
		case 'n':
			newMessage();
			break;		
		case 'v':
			viewChat();
			break;
		case 'u':
			userList();
			break;
		case 'e':
			std::cout << "Exit chat...";
			break;
		default:
			std::cout << "Wrong command. Please type n, r, p, v, u or e... ";
		}
	} while (action != 'e');
}

void Chat::newMessage()
{
	std::cout << "Select message type: \nr - write to the user\np - write to the user privately\nother key - to all\n";
	char messageType{ '\0' };
	bool isPrivateMessage{ false };
	std::string to{ "" };
	messageType = _getche();
	if (messageType == 'p')
	{
		isPrivateMessage = true;
	}

	if (messageType == 'p' || messageType == 'w')
	{
		std::cout << "Input user name: ";
		std::cin >> to;
		if (!checkUserName(to))
		{
			std::cout << "The user named " << to << " was not found...";
			return;
		}
	}

	chatMessages_.push_back(Message{ loginUser_->getName(), to, isPrivateMessage });
}

bool Chat::checkUserLogin(std::string& login)
{
	for (auto& element : chatUsers_)
	{
		if (element.getLogin() == login)
		{
			return true;
		}
	}
	return false;
}

bool Chat::checkUserName(std::string& name)
{
	for (auto& element : chatUsers_)
	{
		if (element.getName() == name)
		{
			return true;
		}
	}
	return false;
}

void Chat::viewChat()
{
	for (auto& element : chatMessages_)
	{
		if (element.getName() == name)
		{
			return true;
		}
	}
	return false;
}

void Chat::userList()
{

}
	