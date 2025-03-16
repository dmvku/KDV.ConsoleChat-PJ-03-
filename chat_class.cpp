#include "chat_class.h"

Chat::~Chat()
{

}

void Chat::registerUser()
{
	std::string login{};
	std::string password{};
	std::string name{};	

	//system("CLS");
	outInformationLines();
	gotoCoordinates(31, 18);
	std::cout << "\033[1;33;44mRegister new user:\033[0m";
	gotoCoordinates(0, 19);
	
	std::cout << "Login: ";
	std::cin >> login;

	list<User>::iterator it = findUser(login);

	try
	{
		if (it != chatUsers_.end())
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
		return;
	}
	
	outInformationLines();
	gotoCoordinates(31, 18);
	std::cout << "\033[1;33;44mRegister new user:\033[0m";
	gotoCoordinates(0, 20);

	std::cout << "Password: ";
	std::cin >> password;
	uint* digest = sha1(password);
	bool valueIsBusy{};

	do
	{
		valueIsBusy = false;
		std::cout << "Name: ";
		std::cin >> name;

		try
		{
			if (checkUserName(name))
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
		}		
	} while (valueIsBusy);
	
	chatUsers_.emplace_back(login, digest, name);
	
	//system("CLS");
	try
	{
		if (chatUsers_.empty())
		{
			throw Warning();
		}
	}
	catch (std::exception& warning)
	{
		gotoCoordinates(24, 18);
		std::cout << warning.what() << "registration failed...\033[0m";
		gotoCoordinates(0, 21);
		std::cout << "\033[2K";
		valueIsBusy = true;
		return;
	}
	gotoCoordinates(lastCoordinateX_, lastCoordinateY_);
	std::cout << "User \033[1;33m" << chatUsers_.back().getName() << "\033[0m is registred...\n";	
	lastCoordinateY_ = getYcoord();
	clearingTheInputWindow();
	outInformationLines();
	outSelectAction();
}

void Chat::deleteUser()
{
	list<User>::iterator it = checkUser("Delete User:");
	if (it != chatUsers_.end())
	{
		gotoCoordinates(lastCoordinateX_, lastCoordinateY_);
		std::cout << "User \033[1;33m" << it->getName() << "\033[0m is deleted...\n";
		lastCoordinateY_ = getYcoord();
		chatUsers_.erase(it);
		outInformationLines();
		outSelectAction();
	}	
	clearingTheInputWindow();
}

void Chat::loginUser()
{	
	list<User>::iterator it = checkUser("User login:");
	if (it != chatUsers_.end())
	{
		loginUser_ = it;
		outInformationLines();
		outSelectAction();
	}		
	clearingTheInputWindow();
}

void Chat::chatMenu()
{
	//system("CLS");
	loginUser();

	char action{};
	if (loginUser_ == chatUsers_.end())
	{
		return;
	}

	TrieNode* root = getNewNode();
	std::string filename{ "dict.txt" };
	read_dict(filename, root);

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
			newMessage(root);
			break;
		case 'v':
			viewChat();
			break;
		case 'u':
			userList();
			break;
		case 'e':
			write_dict(filename, root);
			gotoCoordinates(0, 0);			
			system("CLS");
			loginUser_ = chatUsers_.end();
			break;
		default:
			outInformationLines();
			gotoCoordinates(19, 18);
			std::cout << "\033[1;33;44mWrong command. Please type n, v, u or e...\033[0m";
		}		
	} while (action != 'e');
	lastCoordinateY_ = 0;
	outInformationLines();
	outSelectAction();
}

void Chat::newMessage(TrieNode* root)
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
		if (findUser(to) == chatUsers_.end())
		{
			outInformationLines();
			gotoCoordinates(20, 18);
			std::cout << "\033[1;33;44mThe user named \033[1;36;44m" << to << "\033[1;33;44m was not found...\033[0m";
			return;
		}
	}

	// A message to yourself - a note
	outInformationLines();
	if (to == loginUser_->getName())
	{
		gotoCoordinates(19, 18);
		std::cout << "\033[1;33;44mYou are sending a message to yourself...\033[0m";
		//clearingTheInputWindow();
		//gotoCoordinates(0, 19);
		isPrivateMessage = true;
	}
	else
	{
		gotoCoordinates(30, 18);
		std::cout << "\033[1;33;44mInput message text:\033[0m";
	}
	
	//std::string messageText{ "" };
	//std::cout << "Input message text: ";

	// Create function prefix input
	//std::cin.ignore(1, '\n');
	//std::getline(std::cin, messageText);	
	
	std::string messageText{ creatingMessage(root) };

	chatMessages_.push_back(Message{ loginUser_->getName(), to, messageText,  getTheTimeNow(), isPrivateMessage });

	clearingTheInputWindow();

	if (lastCoordinateY_ == linesLimit_)
	{
		system("CLS");
		lastCoordinateY_ = 0;
	}

	gotoCoordinates(lastCoordinateX_, lastCoordinateY_);

	std::string addMeFrom{ chatMessages_.back().getFrom() == loginUser_->getName() ? "\033[0m(me)" : "" };
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

list<User>::iterator Chat::checkUser(std::string title)
{	
	if (chatUsers_.empty())
	{
		outInformationLines();
		gotoCoordinates(22, 18);
		std::cout << "\033[1;33;44mUsers not found. Please register...\033[0m";
		return chatUsers_.end();
	}
	std::string login{};
	std::string password{};

	outInformationLines();
	gotoCoordinates(33, 18);
	std::cout << "\033[1;33;44m" << title << "\033[0m";
	gotoCoordinates(0, 19);

	clearingTheInputWindow();
	gotoCoordinates(0, 19);
	std::cout << "Login: ";
	std::cin >> login;
	std::cout << "Password: ";
	std::cin >> password;

	list<User>::iterator it = findUser(login);

	try
	{
		if (it != chatUsers_.end())
		{
			uint* digest = sha1(password);
			if (!memcmp(it->getPasswordHash(), digest, SHA1HASHLENGTHBYTES))
			{
				return it;
			}
			else
			{
				throw Warning();
			}
		}
		else
		{
			throw Warning();
		}		
	}
	catch (std::exception& warning)
	{
		clearingTheInputWindow();
		outInformationLines();
		gotoCoordinates(19, 18);
		std::cout << warning.what() << "login or password incorrect...\033[0m";
		//clearingTheInputWindow();
		/*system("CLS");
		outInformationLines();*/
		return chatUsers_.end();
	}
}

list<User>::iterator Chat::findUser(const std::string& _login)
{
	list<User>::iterator it = chatUsers_.begin();

	for (User& user : chatUsers_)
	{
		if (user.getLogin() == _login)
		{
			return it;
		}
		++it;
	}

	return chatUsers_.end();
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
	std::string addFrom{};
	std::string addRecipient{};
	//std::string addTo{};
		
	for (auto& element : chatMessages_)
	{			
		if (element.getFrom() == loginUser_->getName() || element.getTo() == ""
			|| element.getTo() == loginUser_->getName() || !element.getIsPrivateMessage())
		{			
			addFrom = findUser(element.getFrom()) == chatUsers_.end() ? "\033[0mdeleted" : element.getFrom();
			addMeFrom = element.getFrom() == loginUser_->getName() ? "\033[0m(me)" : "";
			if (element.getTo() == "")
			{
				addRecipient = "";
			}			
			else if (element.getTo() == loginUser_->getName())
			{
				addRecipient = "\033[0m to myself";
			}
			else if (findUser(element.getTo()) == chatUsers_.end() && element.getTo() != "")
			{
				addRecipient = "\033[0m to deleted";
			}
			else
			{
				addRecipient = "\033[0m to \033[1;4;36m" + element.getTo();
			}			
			/*addTo = element.getTo() == "" ? "" : "\033[0m to \033[1;4;36m";*/

			std::cout << "\033[1;4;33m" << addFrom << addMeFrom << addRecipient
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
