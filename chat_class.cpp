#include "chat_class.h"

Chat::~Chat()
{

}

void Chat::mainMenu()
{
	readUsersFile();

	char action{ '\0' };
	do
	{
		std::cout << "Select action:\nl - Login\nr - Register\ne - Exit programm\n";
		std::cin >> action;

		switch (action)
		{
		case 'l':
			chatMenu();
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

}

void Chat::registerUser()
{
	std::string login{};
	std::string password{};
	std::string name{};
	bool valueIsBusy{};
	system("clear");
	std::cout << "Register new user:\n";

	do
	{		
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
	std::ofstream fs(usersFile_, ios::app);
	if (!fs)
	{
		fs = ofstream(usersFile_, ios::out | ios::trunc);
	}

	if (fs)
	{
		fs << login << std::endl << password << std::endl << name << std::endl;
	}
	else
	{
		cout << "Could not open file users.data!" << '\n';
		return;
	}

	std::cout << "User \033[1;33m" << chatUsers_.back().getName() << "\033[0m is registred...\n\n";
}

void Chat::loginUser()
{	
	if (chatUsers_.empty())
	{
		std::cout << "Users not found. Please register...\n\n";
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
			std::cout << warning.what() << "login or password incorrect.\nType \"e\" for exit or any key for try again...\n\n";
			char exitLogin{};
			std::cin >> exitLogin;
			std::cout << "\b";
			if (exitLogin == 'e')
			{
				break;
			}
		}		
	} while (!correctUser);		
}

void Chat::chatMenu()
{	
	system("clear");
	loginUser();
	
	char action{ '\0' };
	if (loginUser_ == nullptr)
	{
		return;
	}

	std::cout << "User \033[1;33m" << loginUser_->getName() << "\033[0m is login...\n";
	
	do
	{		
		std::cout << "\nSelect action:\nn - New message\nv - View messages\nu - User list\ne - Exit chat\n";
		std::cin >> action;
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
	system("clear");
	std::cout << "Select message type: \nw - write to the user\np - write to the user privately\nother key - to all\n";
	char messageType{};
	bool isPrivateMessage{ false };
	std::string to{ "" };
	std::cin >> messageType;
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
	system("clear");
	std::cout << "           CHAT:\n";
	std::ifstream fs(messagesFile_, ios::in);
	std::string from{};
	std::string to{};
	std::string message{};
	bool isPrivate{};
	int maxMessagesOnTheScreen{ 10 };
	int countMessagesOnTheScreen{};

	if (!fs)
	{
		cout << "There are no messages in the chat...\n!";		\
	}
	else		
	{
		while (!fs.eof())
		{
			fs >> from >> to >> message >> isPrivate;
			if (from == loginUser_->getName() || to == "" || to == loginUser_->getName() || !isPrivate)
			{
				std::cout << "\033[1;4;33m" << (from == loginUser_->getName() ? "me" : from)
					<< "\033[0m" << (to == "" ? "" : " to \033[1;4;36m" + to)
					<< "\033[0m: " << message << "\n";
			}

			countMessagesOnTheScreen++;

			if (countMessagesOnTheScreen == maxMessagesOnTheScreen)
			{
				char action{ '\0' };
				std::cout << "Continue (y - yes, other key - no)? ";
				std::cin >> action;
				if (action != 'y' || action != 'Y')
				{
					return;
				}

				system("clear");
				std::cout << "           CHAT:\n";
				countMessagesOnTheScreen = 0;
			}
		}
	}		
	
	/*system("clear");
	if (chatMessages_.empty())
	{
		std::cout << "There are no messages in the chat...\n";
		return;
	}
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
	}*/
}

void Chat::userList()
{
	system("clear");
	std::cout << "           User List:\n";
		for (auto& element : chatUsers_)
		{
			std::cout << "\033[1;33m" << element.getName() << "\033[0m\n";
		}
}

void Chat::readUsersFile()
{	
	std::ifstream fs(usersFile_, ios::in);
	std::string login{};
	std::string password{};
	std::string name{};	
	
	if (!fs)
	{
		cout << "Users not found. Please register...\n!";
		return;
	}
	else
	{
		while(!fs.eof())
		{
			fs >> login >> password >> name;
			chatUsers_.push_back(User{ login, password, name });
		}		
	}	
}

void Chat::readMessagesFile()
{
	
}
