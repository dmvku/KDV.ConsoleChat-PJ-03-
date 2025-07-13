#pragma once
#include <string>
#include <vector>

#include "socket.h"
#include "database.h"
#include "user.h"

namespace serverCommand
{
	const std::string helloMsg = "#HELLO|";
	const std::string serviceMsg = "#SERVC|";
	const std::string loginMsg = "#LOGIN|";
	const std::string passwordMsg = "#PASSW|";
	const std::string newMessageMsg = "#NEWMS|";
	const std::string deleteMessageMsg = "#DELMS|";
	const std::string beginUserListMsg = "#BULST|";
	const std::string endUserListMsg = "#EULST";
	const std::string beginChatListMsg = "#BCLST|";
	const std::string endChatListMsg = "#ECLST";
	const std::string exitChatMsg = "#EXITC|";	
	const std::string confirmationMsg = "#OK";
}

namespace textColor
{
	const std::string resetColor = "\033[0m";
	const std::string black = "\033[30m";
	const std::string red = "\033[31m";
	const std::string green = "\033[32m";
	const std::string yellow = "\033[33m";
	const std::string blue = "\033[34m";
	const std::string magenta = "\033[35m";
	const std::string cyan = "\033[36m";
	const std::string white = "\033[37m";
}

namespace userParameters
{
	const std::string ID = "User ID";
	const std::string login = "Login";
	const std::string name = "Name";
	const std::string surname = "Surname";
	const std::string password = "Password";
	const std::string email = "Email";
	const std::string recipient = "Enter the recipient's name";
	const std::string message = "Enter the text of the message";
}

class Chat
{
public:
	Chat();
	~Chat();

	void runChat();

private:	
	struct tempUserData
	{
	public:
		std::string tempID;
		std::string tempLogin;
		std::string tempPasswordHash;
		std::string tempName;
		std::string tempSurname;
		std::string tempEmail;
		std::string tempStatus;
		std::string tempDeliveredMessage;
		std::string tempViewedMessage;
	};

	//tempUserData* tempUser  = new tempUserData;
	Database chatDB;
	Socket server;	
	User loginUser_;
	//std::vector<User> onlineUsers_;	
	
	std::string queryString;	
	
	bool userAutorization();
	bool registerUser();
	bool loginUser();
	void deleteUser();	
	void sendUserList();
	void sendMessages();
	bool chatMenu();
	void newMessage();	
	void deleteMessage();
	bool searchUserByLogin(tempUserData* tempUser);	
	bool searchUserByName(tempUserData* tempUser, std::string& recipient);
	bool checkUserFullName(tempUserData* tempUser);
	void queryPrint();
	void sendDBErrorMessage();
	std::string requestingUserParameter(std::string parameter);
	//std::string requestingName();
	//std::string requestingPassword();
	/*void readDataFile(std::string& file);
	void readUsersFile(std::string& line);
	void readMessagesFile(std::string& line);	
	std::string dataParsing(std::string& data);*/

	 void viewChat();
	 //void viewUserList();
	 //bool requestProcessing(std::string& data);
};
