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
	bool valueIsBusy{};
	std::cout << "Register new user:\n";

	do
	{
		/*std::cout << "What's your name: ";
		std::cin >> name;*/
		valueIsBusy = false;
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
	
	do
	{
		valueIsBusy = false;
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
	std::cout << "User " << chatUsers_.back().getName() << " is registred...\n";
}

void Chat::loginUser()
{
	if (chatUsers_.empty())
	{
		std::cout << "Users not found. Please register...\n";
		return;
	}
	std::string login{};
	std::string password{};
	std::cout << "User login:\n";
	
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
			std::cout << warning.what() << "login or password incorrect.\nType \"e\" for exit or any key for try again...\n";
			char exitLogin{};
			exitLogin = _getche();
			std::cout << "\b";
			if (exitLogin == 'e')
			{
				break;
			}
		}		
	} while (!correctUser);

	/*while (loginUser_)
	{
		chatMenu();
	};*/
}

void Chat::chatMenu()
{	
	loginUser();
	char action{ '\0' };
	if (loginUser_ == nullptr)
	{
		return;
	}
	
	do
	{
		std::cout << "Select action:\nn - New message\nv - View messages\nu - User list\ne - Exit chat\n";
		action = _getche();
		std::cout << "\b";
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
			std::cout << "Exit chat...\n";
			loginUser_ = nullptr;
			break;
		default:
			std::cout << "Wrong command. Please type n, v, u or e... ";
		}
	} while (action != 'e');
}

void Chat::newMessage()
{
	std::cout << "Select message type: \nw - write to the user\np - write to the user privately\nother key - to all\n";
	char messageType{};
	bool isPrivateMessage{ false };
	std::string to{ "" };
	messageType = _getche();
	std::cout << "\b";
	if (messageType == 'p')
	{
		isPrivateMessage = true;
	}

	if (messageType == 'p' || messageType == 'w')
	{
		std::cout << "Input user name: ";
		std::cin >> to;
		if (!checkUserName(to) )
		{
			std::cout << "The user named " << to << " was not found...";
			return;
		}
	}

	// A message to yourself - a note
	if (to == loginUser_->getName())
	{
		std::cout << "You are sending a message to yourself...\n";
		isPrivateMessage = true;
		to = "myself";
	}
	
	std::string messageText{ "" };
	std::cout << "Input massage text:\n";
	std::cin.ignore();
	std::getline(std::cin, messageText);	
	chatMessages_.push_back(Message{ loginUser_->getName(), to, messageText, isPrivateMessage });
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
	std::cout << "           CHAT:\n";
	for (auto& element : chatMessages_)
	{
		if (element.getFrom() == loginUser_->getName() || element.getTo() == ""
			|| element.getTo() == loginUser_->getName() || !element.getIsPrivateMessage())
		{
			std::cout << "\033[1;4;33m" << (element.getFrom() == loginUser_->getName() ? "me" : element.getFrom())
				      << "\033[0m" << (element.getTo() == "" ? "" : " to \033[1;4;36m" + element.getTo())
				      << "\033[0m: " << element.getMessage() << "\n";
		}		
	}
}

void Chat::userList()
{
	std::cout << "           User List:\n";
		for (auto& element : chatUsers_)
		{
			std::cout << "\033[1;33m" << element.getName() << "\033[0m\n";
		}
}
	