#include "chat_class.h"

Chat::~Chat()
{

}

void Chat::registerUser()
{
	std::string login{};
	std::string password{};
	std::string name{};
	bool valueIsBusy{};
	system("CLS");
	outInformationLines();
	gotoCoordinates(31, 18);
	std::cout << "\033[1;33;44mRegister new user:\033[0m";
	gotoCoordinates(0, 19);
	
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
				gotoCoordinates(13, 18);
				std::cout << warning.what() << "login is busy. Choose a different login...\033[0m";
				gotoCoordinates(0, 19);
				std::cout << "\033[2K";
				valueIsBusy = true;				
				break;
			}
		}
	} while (valueIsBusy);
	
	outInformationLines();
	gotoCoordinates(31, 18);
	std::cout << "\033[1;33;44mRegister new user:\033[0m";
	gotoCoordinates(0, 20);
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
				gotoCoordinates(14, 18);
				std::cout << warning.what() << "name is busy. Choose a different name...\033[0m";
				gotoCoordinates(0, 21);
				std::cout << "\033[2K";
				valueIsBusy = true;
				break;
			}
		}
	} while (valueIsBusy);

	chatUsers_.push_back(User{ login, password, name });
	system("CLS");
	std::cout << "User \033[1;33m" << chatUsers_.back().getName() << "\033[0m is registred...\n\n";
	outInformationLines();
}

void Chat::loginUser()
{	
	if (chatUsers_.empty())
	{
		outInformationLines();
		gotoCoordinates(22, 18);		
		std::cout << "\033[1;33;44mUsers not found. Please register...\033[0m";
		return;
	}
	std::string login{};
	std::string password{};
	
	bool correctUser = false;
	do
	{
		system("CLS");
		outInformationLines();
		gotoCoordinates(35, 18);
		std::cout << "\033[1;36;44mUser login:\033[0m";
		gotoCoordinates(0, 19);
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
			clearingTheInputWindow(); 
			outInformationLines();
			gotoCoordinates(19, 18);
			std::cout << warning.what() << "login or password incorrect...";
			gotoCoordinates(24, 24);
			std::cout << "\033[1;44m\033[36me\033[33mxit    \033[36mother key\033[33m - try again\033[0m";				
			gotoCoordinates(0, 19);
							
			char exitLogin{};
			exitLogin = _getche();
			
			if (exitLogin == 'e')
			{
				system("CLS");
				outInformationLines();
				break;
			}
		}		
	} while (!correctUser);	
	outSelectAction();
}

void Chat::chatMenu()
{
	system("CLS");
	loginUser();

	char action{};
	if (loginUser_ == nullptr)
	{
		return;
	}

	system("CLS");
	outInformationLines();
	outSelectAction();
	gotoCoordinates(0, 0);
	std::cout << "User \033[1;33m" << loginUser_->getName() << "\033[0m is login...\n";
	lastCoordinateY_ = getYcoord();

	do
	{
		gotoCoordinates(15, 24);
		std::cout << "\033[1;44m\033[36mn\033[33mew message    \033[36mv\033[33miew message    "
			<< "\033[36mu\033[33mser list    \033[36me\033[33mxit\033[0m";
		clearingTheInputWindow();
		gotoCoordinates(0, 19);

		action = _getche();
		std::cout << "\033[2K";
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
			gotoCoordinates(0, 0);			
			system("CLS");
			loginUser_ = nullptr;
			break;
		default:
			outInformationLines();
			gotoCoordinates(19, 18);
			std::cout << "\033[1;33;44mWrong command. Please type n, v, u or e...\033[0m";
		}		
	} while (action != 'e');
	outInformationLines();
	outSelectAction();
}

void Chat::newMessage()
{
	clearingTheInputWindow();
	outInformationLines();
	gotoCoordinates(30, 18);
	std::cout << "\033[1;33;44mSelect message type:\033[37; 0m";
	gotoCoordinates(5, 24);
	std::cout << "\033[1;33;44m\033[36mw\033[33mrite to the user    write to the user \033[36mp"
		      << "\033[33mrivately    \033[36mother key\033[33m to all\033[37;0m";
	gotoCoordinates(0, 19);
	
	char messageType{};
	bool isPrivateMessage{ false };
	std::string to{ "" };              // "" - message to all users.

	messageType = _getche();
	std::cout << "\b\033[2K";
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
			outInformationLines();
			gotoCoordinates(20, 18);
			std::cout << "\033[1;33;44mThe user named \033[1;36;44m" << to << "\033[1;33;44m was not found...\033[0m";
			return;
		}
	}

	// A message to yourself - a note
	if (to == loginUser_->getName())
	{
		std::cout << "You are sending a message to yourself...\n";
		isPrivateMessage = true;
	}
	
	std::string messageText{ "" };
	std::cout << "Input message text:\n";
	std::cin.ignore();
	std::getline(std::cin, messageText);	
	chatMessages_.push_back(Message{ loginUser_->getName(), to, messageText,  getTheTimeNow(), isPrivateMessage });

	clearingTheInputWindow();

	if (lastCoordinateY_ == linesLimit_)
	{
		system("CLS");
		lastCoordinateY_ = 0;
	}

	gotoCoordinates(lastCoordinateX_, lastCoordinateY_);

	std::string addMeFrom{ chatMessages_.back().getFrom() == loginUser_->getName() ? "(me)" : "" };
	std::string addRecipient{ chatMessages_.back().getTo()
		== loginUser_->getName() ? "\033[0mmyself" : chatMessages_.back().getTo() };
	std::string addTo{ chatMessages_.back().getTo() == "" ? "" : "\033[0m to \033[1;4;36m" };

	std::cout << "\033[1;4;33m" << chatMessages_.back().getFrom() << addMeFrom << addTo
		      << addRecipient << "\033[0m \033[1;4;32m" << chatMessages_.back().getCreationTime()
		      << "\033[0m: " << chatMessages_.back().getMessage() << "\n";		    

	lastCoordinateY_ = getYcoord();
	outInformationLines();
	outSelectAction();
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
	system("CLS");
	outInformationLines();
	
	if (chatMessages_.empty())
	{
		outInformationLines();
		gotoCoordinates(21, 18);
		std::cout << "\033[1;33;44mThere are no messages in the chat...\033[0m";
		return;
	}

	gotoCoordinates(37, 18);
	std::cout << "\033[1;33;44mCHAT\033[0m";
	gotoCoordinates(0, 0);
	std::string addMeFrom{};
	std::string addRecipient{};
	std::string addTo{};
		
	for (auto& element : chatMessages_)
	{			
		if (element.getFrom() == loginUser_->getName() || element.getTo() == ""
			|| element.getTo() == loginUser_->getName() || !element.getIsPrivateMessage())
		{			
			addMeFrom = element.getFrom() == loginUser_->getName() ? "(me)" : "";
			addRecipient = element.getTo() == loginUser_->getName() ? "\033[0mmyself" : element.getTo();
			addTo = element.getTo() == "" ? "" : "\033[0m to \033[1;4;36m";

			std::cout << "\033[1;4;33m" << element.getFrom() << addMeFrom << addTo << addRecipient
				      << "\033[0m \033[1;4;32m" << element.getCreationTime() << "\033[0m: " 
				      << element.getMessage() << "\n";
			lastCoordinateY_ = getYcoord();
		}

		if (lastCoordinateY_ >= linesLimit_)
		{				
			gotoCoordinates(22, 24);
			std::cout << "\033[1;44m\033[36mn\033[33mext page    \033[36mother key\033[33m - chat menu\033[0m";
			gotoCoordinates(0, 19);
			char action{};
			action = _getche();
			std::cout << "\033[2K";
			if (action == 'n')
			{
				std::cout << "\033[1J";
				lastCoordinateY_ = 0;
				outInformationLines();
				gotoCoordinates(38, 18);
				std::cout << "\033[1;33;44mCHAT\033[0m";
				gotoCoordinates(0, 0);
				continue;
			}			
			return;
		}
	}
}

void Chat::userList()
{
	system("CLS");
	outInformationLines();
	gotoCoordinates(35, 18);
	std::cout << "\033[1;33;44mUser List\033[0m";
	gotoCoordinates(0, 0);
	std::string addMe{};
	
		for (auto& element : chatUsers_)
		{
			addMe = element.getName() == loginUser_->getName() ? "(me)" : "";
			std::cout << "\033[1;33m" << element.getName() << "\033[0m" << addMe << " \n";
			lastCoordinateY_ = getYcoord();
			std::cout << "\033[2K";
			if (lastCoordinateY_ >= linesLimit_)
			{
				gotoCoordinates(22, 24);
				std::cout << "\033[1;44m\033[36mn\033[33mext page    \033[36mother key\033[33m - chat menu\033[0m";
				gotoCoordinates(0, 19);
				char action{};
				action = _getche();

				if (action == 'n')
				{
					std::cout << "\033[1J";
					lastCoordinateY_ = 0;
					outInformationLines();
					gotoCoordinates(35, 18);
					std::cout << "\033[1;33;44mUser List\033[0m";
					gotoCoordinates(0, 0);
					continue;
				}
				return;
			}
		}
}

std::string Chat::getTheTimeNow()
{
	time_t systemTime = time(0);
	tm* timeNow = localtime(&systemTime);
	char timeBuffer[25];
	strftime(timeBuffer, sizeof(timeBuffer), "%d-%m-%y %H:%M:%S", timeNow);
	std::string dateAndTime = timeBuffer;
	return dateAndTime;
}
