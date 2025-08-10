#include "chat.h"
#include "socket.h"
#include "database.h"

#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <shared_mutex>

Chat::Chat()
{   	
	//Logger chatLogger;
}

Chat::~Chat()
{
	//delete tempUser;
}

void Chat::runChat()
{
	if (chatDB.mysqlError || !server.getPort())
	{
		return;
	}
	
	logger_.readingLog();   ////////////////////////////////////////////////////////////
	std::cout << "Run chat...\n";

	while (true)
	{
		server.createConnection();
		
		// Call to register or login
		if (!userAutorization())
		{		
			server.sendData_ = serverCommand::exitChatMsg;			
			server.dataTransmission();

			chatLogLine_ = "Socket "
				+ std::to_string(server.connection)
				+ " - exit autorization... "
				+ server.socketSendResultLog_;
			logger_.createLogLine(chatLogLine_);

			server.closeConnection();
			continue;
		}

		sendUserList();
		sendMessages();

		if (!chatMenu())
		{
			//std::cout << "End chat...\n";			
			server.stopServer();			
			return;
		}
		else
		{
			//std::cout << "Logout...\n";			
			loginUser_.clearUser();
			server.closeConnection();
		}
	}	
}

bool Chat::userAutorization()
{
	//std::cout << "Register...\n";	
	
	server.sendData_ = serverCommand::helloMsg
		+ textColor::green
		+ "Hello!\nPlease (l)ogin, (r)egister, (d)elete user " 
		+ "or other key for exit: "
		+ textColor::resetColor;	
	server.dataTransmission();

	chatLogLine_ = "Socket "
		+ std::to_string(server.connection)
		+ " - command 'Autorization menu'. Send menu. "
		+ server.socketSendResultLog_;
	logger_.createLogLine(chatLogLine_);

	server.dataRecieving();	
			
	switch (server.message[0])
	{
	case 'l':
		chatLogLine_ = "Socket "
			+ std::to_string(server.connection)
			+ " - command 'Login'. Query the database for a non-empty.";
		logger_.createLogLine(chatLogLine_);

		queryString = std::string("SELECT COUNT(*) FROM users");
		//queryPrint();
		if (chatDB.databaseQuery(queryString))
		{				
			logger_.createLogLine("Query is done.");
			if (chatDB.resultQuery_.empty()
				|| std::stoi(chatDB.resultField_) <= 1) // "to_all" is exist
			{
				server.sendData_ = serverCommand::serviceMsg
					+ textColor::red
					+ "Users not found. Please register...\n"
					+ textColor::resetColor;				
				server.dataTransmission();

				chatLogLine_ = "Socket "
					+ std::to_string(server.connection)
					+ " - Database: users not found. Sent warning. Go register. "
					+ server.socketSendResultLog_;
				logger_.createLogLine(chatLogLine_);

				if (!registerUser())
				{
					return false;
				}
			}	
			return loginUser();			
		}
		else
		{
			logger_.createLogLine(chatDB.databaseLog_.str());
			sendDBErrorMessage();
			return false;
		}			
	case 'r':
		chatLogLine_ = "Socket "
			+ std::to_string(server.connection)
			+ " - command 'Register'.";
		logger_.createLogLine(chatLogLine_);

		if (!registerUser())
		{
			return false;
		}
		return loginUser();
	case 'd':
		chatLogLine_ = "Socket "
			+ std::to_string(server.connection)
			+ " - command 'Delete user'.";
		logger_.createLogLine(chatLogLine_);

		deleteUser();
		return false;
	default:
		server.sendData_ = serverCommand::exitChatMsg;
		server.dataTransmission();

		chatLogLine_ = "Socket "
			+ std::to_string(server.connection)
			+ " - sent 'exit autorization'. "
			+ server.socketSendResultLog_;
		logger_.createLogLine(chatLogLine_);

		return false;
	}
}

bool Chat::registerUser()
{	
	tempUserData* tempUser = new tempUserData;
	bool isFoundUser{ false };
	//bool valueIsBusy{ false };

	server.sendData_ = serverCommand::loginMsg;
	server.dataTransmission();		
	//std::cout << "Register user...\n";
	chatLogLine_ = "Socket "
		+ std::to_string(server.connection)
		+ " - Register user."
		+ server.socketSendResultLog_;
	logger_.createLogLine(chatLogLine_);	
	
	do
	{
		//valueIsBusy = false;		
		tempUser->tempLogin = 
			requestingUserParameter(userParameters::login);
		isFoundUser = searchUserByLogin(tempUser);
		if (isFoundUser)
		{
			server.sendData_ = serverCommand::serviceMsg
				+ textColor::red
				+ "Error: login '" + tempUser->tempLogin + "' is busy."
				+ "Ñhoose a different login...\n"
				+ textColor::resetColor;
			std::cout << server.sendData_;
			server.dataTransmission();	

			chatLogLine_ = "Socket "
				+ std::to_string(server.connection)
				+ " - login is busy. Sent warning. "
				+ server.socketSendResultLog_;
			logger_.createLogLine(chatLogLine_);
		}
	} while (isFoundUser);
		
	tempUser->tempPasswordHash = 
		requestingUserParameter(userParameters::password);

	do
	{		
		tempUser->tempName =
			requestingUserParameter(userParameters::name);
		tempUser->tempSurname =
			requestingUserParameter(userParameters::surname);

		isFoundUser = checkUserFullName(tempUser);
		if (isFoundUser)
		{
			server.sendData_ = serverCommand::serviceMsg
				+ textColor::red
				+ "Error: name '" + tempUser->tempName + " "
				+ tempUser->tempSurname
				+ "' is busy.Ñhoose a different name...\n"
				+ textColor::resetColor;
			std::cout << server.sendData_;
			server.dataTransmission();	

			chatLogLine_ = "Socket "
				+ std::to_string(server.connection)
				+ " - name is busy. Sent warning. "
				+ server.socketSendResultLog_;
			logger_.createLogLine(chatLogLine_);
		}		
	} while (isFoundUser);

	tempUser->tempEmail =
		requestingUserParameter(userParameters::email);
	if (std::string(server.message).empty())
	{
		tempUser->tempEmail = "empty";
	}
	else
	{
		tempUser->tempEmail = std::string(server.message);
	}
	
	// DB. Add user
	queryString = std::string("INSERT INTO ")
		+ "users (login, name, surname, email) "
		+ "VALUES ('"
		+ tempUser->tempLogin + "', '"
		+ tempUser->tempName + "', '"
		+ tempUser->tempSurname + "', '"
		+ tempUser->tempEmail + "')";
	//std::cout << queryString << "\n";
	//queryPrint(); 
	chatLogLine_ = "Socket "
		+ std::to_string(server.connection)
		+ " - Database: Query to add a user.";
	logger_.createLogLine(chatLogLine_);

	if (!chatDB.databaseQuery(queryString))
	{
		sendDBErrorMessage();
		logger_.createLogLine(chatDB.databaseLog_.str());
		return false;
	}	

	if (checkUserFullName(tempUser))
	{		
		queryString = std::string("INSERT INTO ")
			+ "users_password (user_id, password_hash) "
			+ "VALUES (" + tempUser->tempID + ", '"
			+ tempUser->tempPasswordHash + "')";

		chatLogLine_ = "Socket "
			+ std::to_string(server.connection)
			+ " - Database: Query to add the users password.";
		logger_.createLogLine(chatLogLine_);

		//queryPrint();
		if (!chatDB.databaseQuery(queryString))
		{
			sendDBErrorMessage();
			logger_.createLogLine(chatDB.databaseLog_.str());
			return false;
		}

		server.sendData_ = serverCommand::serviceMsg
			+ textColor::green + "User " + tempUser->tempName + " "
			+ tempUser->tempSurname + " is registred.\n"
			+ textColor::resetColor;		
		server.dataTransmission();

		chatLogLine_ = "Socket "
			+ std::to_string(server.connection)
			+ " - user " 
			+ tempUser->tempID 
			+ " is registred. "
			+ server.socketSendResultLog_;
		logger_.createLogLine(chatLogLine_);
	}
	else
	{
		sendDBErrorMessage();
		logger_.createLogLine(chatDB.databaseLog_.str());
	}

	// connection socket	
	delete tempUser;
	return true;
}

bool Chat::loginUser()
{	
	tempUserData* tempUser = new tempUserData;
	bool isFoundUser{ false };	
	std::string password;

	server.sendData_ = serverCommand::loginMsg;		
	server.dataTransmission();
	//std::cout << "Login user...\n";
	chatLogLine_ = "Socket "
		+ std::to_string(server.connection)
		+ " - authorization query. "
		+ server.socketSendResultLog_;
	logger_.createLogLine(chatLogLine_);

	do
	{
		tempUser->tempLogin = requestingUserParameter(userParameters::login);
		password = requestingUserParameter(userParameters::password);

		if (searchUserByLogin(tempUser)
			&& tempUser->tempStatus != "0"
			&& password == tempUser->tempPasswordHash)
		{
			loginUser_.setUserID(tempUser->tempID);
			server.sendData_ = serverCommand::loginMsg
				+ tempUser->tempID + "|"
				+ tempUser->tempName + "|"
				+ tempUser->tempSurname + "|";
			server.dataTransmission();

			isFoundUser = true;
		}
		
		if (!isFoundUser)
		{
			server.sendData_ = textColor::red
				+ "Error: login or password incorrect. "
				+ "Type(e) for exit or any key for try again...\n"
				+ textColor::resetColor;
			server.dataTransmission();

			chatLogLine_ = "Socket "
				+ std::to_string(server.connection)
				+ " - users login or password incorrect. Sent warning. "				
				+ server.socketSendResultLog_;
			logger_.createLogLine(chatLogLine_);

			server.dataRecieving();

			if (server.message[0] == 'e')
			{				
				return false;
			}			
		}	

		loginUser_.setDeliveredMessage(tempUser->tempDeliveredMessage);
		loginUser_.setViewedMessage(tempUser->tempViewedMessage);

	} while (!isFoundUser);

	delete tempUser;

	chatLogLine_ = "Socket "
		+ std::to_string(server.connection)
		+ ": user "
		+ loginUser_.getUserID()
		+ " is login. Sent user data. "
		+ server.socketSendResultLog_;
	logger_.createLogLine(chatLogLine_);

	return true;
}

void Chat::deleteUser()
{	
	tempUserData* tempUser = new tempUserData;
	bool isFoundUser{ false };	
	std::string password;

	server.sendData_ = serverCommand::loginMsg;
	server.dataTransmission();

	chatLogLine_ = "Socket "
		+ std::to_string(server.connection)
		+ " - deleting user. " 
		+ server.socketSendResultLog_;
	logger_.createLogLine(chatLogLine_);
	//std::cout << "Deleting user...\n";

	tempUser->tempLogin = requestingUserParameter(userParameters::login);
	password = requestingUserParameter(userParameters::password);

	if (searchUserByLogin(tempUser)
		&& tempUser->tempStatus != "0"
		&& password == tempUser->tempPasswordHash)
	{
		queryString = std::string("UPDATE users ")
			+ "SET status = 0 WHERE user_id = " + tempUser->tempID;
		//queryPrint();

		chatLogLine_ = "Socket "
			+ std::to_string(server.connection)
			+ " - Database: Query to deleting a user.";
		logger_.createLogLine(chatLogLine_);

		if (!chatDB.databaseQuery(queryString))
		{
			sendDBErrorMessage();
			logger_.createLogLine(chatDB.databaseLog_.str());
		}
		
		queryString = std::string("UPDATE users_password ")
			+ "SET password_hash = '0' WHERE user_id = " + tempUser->tempID;
		//queryPrint();

		chatLogLine_ = "Socket "
			+ std::to_string(server.connection)
			+ " - Database: Query to deleting the users password.";
		logger_.createLogLine(chatLogLine_);

		if (chatDB.databaseQuery(queryString))
		{
			server.sendData_ = serverCommand::serviceMsg
				+ textColor::red + "User "
				+ tempUser->tempLogin + " has been deleted.\n"
				+ textColor::resetColor;

			chatLogLine_ = "Socket "
				+ std::to_string(server.connection)
				+ " - user is deleted..";
			logger_.createLogLine(chatLogLine_);
			logger_.createLogLine("..");
			server.dataTransmission();
		}
		else
		{
			sendDBErrorMessage();
			logger_.createLogLine(chatDB.databaseLog_.str());
		}
	}
	else
	{
		server.sendData_ = textColor::red
			+ "Error: login or password is incorrect "
			+ "or the user has been deleted earlier...\n"
			+ textColor::resetColor;
		server.dataTransmission();

		chatLogLine_ = "Socket "
			+ std::to_string(server.connection)
			+ " - invalid username or password to delete the user. "
			+ server.socketSendResultLog_;
		logger_.createLogLine(chatLogLine_);
		//server.dataRecieving();
	}	
	delete tempUser;
	return;
}

void Chat::sendUserList()
{			
	queryString = std::string("SELECT * FROM view_user_list");
	// queryPrint();
	
	chatLogLine_ = "User "
		+ loginUser_.getUserID()
		+ " - Database: Query a userlist. ";		
	logger_.createLogLine(chatLogLine_);
	
	if (chatDB.databaseQuery(queryString))
	{
		if (chatDB.resultQuery_.empty())
		{
			return;
		}
		
		server.sendData_ = serverCommand::beginUserListMsg;
		server.dataTransmission();

		for (auto row : chatDB.resultQuery_)
		{
			server.sendData_ = std::string(row.at(0)) + "|"
				+ std::string(row.at(1)) + "|"
				+ std::string(row.at(2)) + "|";
			server.dataTransmission();

			do
			{
				server.dataRecieving();
				if (server.message == serverCommand::confirmationMsg)
				{
					break;
				}
			} while (true);
		}
		
		server.sendData_ = serverCommand::endUserListMsg;			
		server.dataTransmission();

		chatLogLine_ = "User "
			+ loginUser_.getUserID()
			+ " - user list is sended. "
			+ server.socketSendResultLog_;
		logger_.createLogLine(chatLogLine_);
	}
	else
	{
		sendDBErrorMessage();
		logger_.createLogLine(chatDB.databaseLog_.str());
	}	
}

void Chat::sendMessages()
{		
	std::string userID = loginUser_.getUserID();	
	std::string lastMessageID;

	queryString = std::string("SELECT * FROM chat_list ")
		+ "WHERE sender_id = " + userID
		+ " OR recipient_id = " + userID
		+ " OR recipient_id = 1 "
		+ "ORDER BY time_create ASC";	

	chatLogLine_ = "User "
		+ loginUser_.getUserID()
		+ " - Database: Query messages. ";
	logger_.createLogLine(chatLogLine_);

	//queryPrint();	
	if (chatDB.databaseQuery(queryString))
	{
		if (chatDB.resultQuery_.empty())
		{
			return;
		}
		
		server.sendData_ = serverCommand::beginChatListMsg;
		server.dataTransmission();

		if (!chatDB.resultQuery_.empty())
		{
			for (auto row : chatDB.resultQuery_)
			{
				lastMessageID = std::string(row.at(0));
				server.sendData_ = lastMessageID + "|"
					+ std::string(row.at(1)) + "|"
					+ std::string(row.at(2)) + "|"
					+ std::string(row.at(3)) + "|"
					+ std::string(row.at(4)) + "|";
				server.dataTransmission();

				do
				{
					server.dataRecieving();
					if (server.message == serverCommand::confirmationMsg)
					{
						break;
					}
				} while (true);
			}
			server.sendData_ = serverCommand::endChatListMsg;
			server.dataTransmission();

			chatLogLine_ = "User "
				+ loginUser_.getUserID()
				+ " - messages is sended. "
				+ server.socketSendResultLog_;
			logger_.createLogLine(chatLogLine_);			
			
			queryString = std::string("UPDATE users ")
				+ "SET delivered_message = " + lastMessageID
				+ " WHERE user_id = " + userID;

			chatLogLine_ = "User "
				+ loginUser_.getUserID()
				+ " - Database: set the last delivered message. ";
			logger_.createLogLine(chatLogLine_);

			//queryPrint();
			if (!chatDB.databaseQuery(queryString))
			{
				sendDBErrorMessage();
			}
			loginUser_.setDeliveredMessage(lastMessageID);
		}
	}
	else
	{
		sendDBErrorMessage();
		logger_.createLogLine(chatDB.databaseLog_.str());
	}	
}

bool Chat::chatMenu()
{
	server.sendData_ = serverCommand::serviceMsg
		+ textColor::green
		+ "Welcome to chat!\n"
		+ textColor::resetColor;			
	server.dataTransmission();

	chatLogLine_ = "User "
		+ loginUser_.getUserID()
		+ " - sent welcoming. ";	
	logger_.createLogLine(chatLogLine_);

	do
	{
		//std::cout << "Main menu...\n";
		server.sendData_ = textColor::cyan
			+ "\nSelect action:\nn - New message\nd - delete message"
			+ "\nv - View messages\nu - User list"
			+ "\nl - Logout (exit client)\nother - Stop chat server\n"
			+ textColor::resetColor;
		server.dataTransmission();

		chatLogLine_ = "User "
			+ loginUser_.getUserID()
			+ " - sent chat menu. ";
		logger_.createLogLine(chatLogLine_);

		server.dataRecieving();

		switch (server.message[0])
		{
		case 'n':
			chatLogLine_ = "User "
				+ loginUser_.getUserID()
				+ " - create new message. ";
			logger_.createLogLine(chatLogLine_);

			newMessage();
			break;
		case 'd':
			chatLogLine_ = "User "
				+ loginUser_.getUserID()
				+ " - delete message. ";
			logger_.createLogLine(chatLogLine_);

			deleteMessage();
			break;
		case 'v':
			//std::cout << "User is viewed messages...\n";

			chatLogLine_ = "User "
				+ loginUser_.getUserID()
				+ " - viewed messages. ";
			logger_.createLogLine(chatLogLine_);
			
			viewChat();
			break;
		case 'u':
			//std::cout << "User is viewed userlist...\n";

			chatLogLine_ = "User "
				+ loginUser_.getUserID()
				+ " - viewed userlist. ";
			logger_.createLogLine(chatLogLine_);
			
			break;
		case 'l':
			server.sendData_ = serverCommand::exitChatMsg;
			server.dataTransmission();

			chatLogLine_ = "User "
				+ loginUser_.getUserID()
				+ " - command 'Exit chat'";
			logger_.createLogLine(chatLogLine_);

			//server.closeConnection();
			return 1;			
		default:
			server.sendData_ = serverCommand::exitChatMsg;
			server.dataTransmission();

			chatLogLine_ = "User "
				+ loginUser_.getUserID()
				+ " - command 'Logout'";
			logger_.createLogLine(chatLogLine_);

			//server.stopServer();
			return 0;				
		}
	} while (true);
}

void Chat::newMessage()
{	
	tempUserData* tempUser = new tempUserData;
	std::string sender = loginUser_.getUserID();
	std::string recipient{ "to_all" };
	std::string createTime;
	
	server.sendData_ = serverCommand::newMessageMsg;
	server.dataTransmission();	
	//std::cout << "\nNew messages...\n";

	server.sendData_ = textColor::cyan
		+ "Select message type: \nw - write to the user\nother key - to all\n"
		+ textColor::resetColor;
	server.dataTransmission();

	chatLogLine_ = "User "
		+ loginUser_.getUserID()
		+ " - sent new message menu.";
	logger_.createLogLine(chatLogLine_);

	server.dataRecieving();	
	
	if (server.message[0] == 'w')
	{		
		recipient = requestingUserParameter(userParameters::recipient);
	}
	
	if (searchUserByName(tempUser, recipient) && tempUser->tempStatus != "0")
	{
		recipient = tempUser->tempID;
	}
	else	
	{
		server.sendData_ = serverCommand::serviceMsg
			+ textColor::red
			+ "User not found!\n"
			+ textColor::resetColor;
		server.dataTransmission();

		chatLogLine_ = "User "
			+ loginUser_.getUserID()
			+ " - sent 'User not found'.";
		logger_.createLogLine(chatLogLine_);

		return;
	}
	
	std::string messageText = 
		requestingUserParameter(userParameters::message);
	if (messageText.empty())
	{
		server.sendData_ = serverCommand::serviceMsg
			+ textColor::red
		    + "The message is empty...\n"
			+ textColor::resetColor;
		server.dataTransmission();

		chatLogLine_ = "User "
			+ loginUser_.getUserID()
			+ " - sent 'The message is empty'.";
		logger_.createLogLine(chatLogLine_);

		return;
	}
	
	queryString = std::string("INSERT INTO messages ")
		+ "(sender_id, recipient_id, message) "
		+ "VALUE (" + sender + ", " + recipient + ", '" + messageText + "')";

	chatLogLine_ = "User "
		+ loginUser_.getUserID()
		+ " - Database: added new message. ";
	logger_.createLogLine(chatLogLine_);

	//queryPrint();	
	if (!chatDB.databaseQuery(queryString))
	{
		sendDBErrorMessage();
		logger_.createLogLine(chatDB.databaseLog_.str());
	}
	
	queryString = std::string("SELECT max(time_create) FROM chat_list");	

	chatLogLine_ = "User "
		+ loginUser_.getUserID()
		+ " - Database: query the time to create a new message. ";
	logger_.createLogLine(chatLogLine_);

	//queryPrint();
	if (chatDB.databaseQuery(queryString))
	{
		if (!chatDB.resultQuery_.empty())
		{			
			createTime = chatDB.resultField_;
		}
	}
	else
	{
		sendDBErrorMessage();
		logger_.createLogLine(chatDB.databaseLog_.str());
	}

	queryString = std::string("SELECT * FROM chat_list ")
		+ "WHERE sender_id = " + sender
		+ " AND recipient_id = " + recipient
		+ " AND message = '" + messageText
		+ "' AND time_create = '" + createTime + "'";	

	chatLogLine_ = "User "
		+ loginUser_.getUserID()
		+ " - Database: query a new message to send. ";
	logger_.createLogLine(chatLogLine_);

	//queryPrint();
	if (chatDB.databaseQuery(queryString))
	{		
		if (!chatDB.resultQuery_.empty())
		{			
			server.sendData_ = serverCommand::newMessageMsg
				+ std::string(chatDB.resultRow_.at(0)) + "|"
				+ std::string(chatDB.resultRow_.at(1)) + "|"
				+ std::string(chatDB.resultRow_.at(2)) + "|"
				+ std::string(chatDB.resultRow_.at(3)) + "|"
				+ std::string(chatDB.resultRow_.at(4)) + "|";
			server.dataTransmission();

			chatLogLine_ = "User "
				+ loginUser_.getUserID()
				+ " - sent new message. ";
			logger_.createLogLine(chatLogLine_);
		}
	}
	else
	{
		sendDBErrorMessage();
		logger_.createLogLine(chatDB.databaseLog_.str());
	}

	delete tempUser;
}

void Chat::deleteMessage()
{	
	std::string deletingMessageID;	
	std::string senderID;	

	server.sendData_ = serverCommand::deleteMessageMsg;
	server.dataTransmission();

	chatLogLine_ = "User "
		+ loginUser_.getUserID()
		+ " - sent command 'Delete message'. ";
	logger_.createLogLine(chatLogLine_);

	server.sendData_ = serverCommand::beginUserListMsg;		
	server.dataTransmission();
	server.dataRecieving();
	deletingMessageID = std::string(server.message);
	if (deletingMessageID == "0")
	{
		server.sendData_ = serverCommand::serviceMsg
			+ "Return to the main menu...\n";
		server.dataTransmission();
		return;
	}

	queryString = std::string("SELECT COUNT(*) ")		
		+ "FROM messages "
		+ "WHERE message_ID = " + deletingMessageID
		+ " AND sender_id = " + loginUser_.getUserID()
		+ " AND status != 0";

	chatLogLine_ = "User "
		+ loginUser_.getUserID()
		+ " - Database: query to search for a message to delete. ";
	logger_.createLogLine(chatLogLine_);

	//queryPrint();
	if (chatDB.databaseQuery(queryString))
	{		
		if (chatDB.resultQuery_.empty() || chatDB.resultField_ == "0")
		{
			server.sendData_ = serverCommand::serviceMsg
				+ textColor::red + "The message ID "
				+ deletingMessageID + " was not found or deleted...\n"
				+ textColor::resetColor;
			server.dataTransmission();	

			chatLogLine_ = "User "
				+ loginUser_.getUserID()
				+ " - Database: message is not found. Sent warning.";
			logger_.createLogLine(chatLogLine_);

			return;
		}		
	}
	else
	{
		sendDBErrorMessage();
		logger_.createLogLine(chatDB.databaseLog_.str());
		return;
	}	

	queryString = std::string("UPDATE messages SET status = 0 ")
		+ "WHERE message_id = " + deletingMessageID;

	if (!chatDB.databaseQuery(queryString))
	{
		sendDBErrorMessage();
		logger_.createLogLine(chatDB.databaseLog_.str());
		return;
	}

	server.sendData_ = serverCommand::deleteMessageMsg
		+ deletingMessageID;
	server.dataTransmission();

	chatLogLine_ = "User "
		+ loginUser_.getUserID()
		+ " - Database: the message is deleted. ";
	logger_.createLogLine(chatLogLine_);
}

bool Chat::searchUserByLogin(tempUserData* tempUser)
{	
	queryString = std::string("SELECT user_id, name, surname, ")
		+ "status, delivered_message, viewed_message "
		+ "FROM users WHERE login = '" + tempUser->tempLogin + "'";	

	chatLogLine_ = "Socket "
		+ std::to_string(server.connection)
		+ " - Database: user search query. Login: "
		+ tempUser->tempLogin;
	logger_.createLogLine(chatLogLine_);

	//queryPrint();
	if (chatDB.databaseQuery(queryString))
	{		
		if (!chatDB.resultQuery_.empty())
		{
			tempUser->tempID = chatDB.resultQuery_.at(0).at(0);
			tempUser->tempName = chatDB.resultQuery_.at(0).at(1);
			tempUser->tempSurname = chatDB.resultQuery_.at(0).at(2);
			tempUser->tempStatus = chatDB.resultQuery_.at(0).at(3);
			tempUser->tempDeliveredMessage = chatDB.resultQuery_.at(0).at(4);
			tempUser->tempViewedMessage = chatDB.resultQuery_.at(0).at(5);

			chatLogLine_ = "User "
				+ tempUser->tempID
				+ " - Database: user is found. ";
			logger_.createLogLine(chatLogLine_);
			//std::cout << "User " << tempUser->tempLogin << " is found.";
		}
		else
		{
			chatLogLine_ = "Socket "
				+ std::to_string(server.connection)
				+ " - Database: user is not found. ";
			logger_.createLogLine(chatLogLine_);

			//std::cout << "User " << tempUser->tempLogin << " isn't found.\n";
			return false;
		}		
	}
	else
	{
		sendDBErrorMessage();		
		logger_.createLogLine(chatDB.databaseLog_.str());
	}

	queryString = std::string("SELECT password_hash ")		
		+ "FROM users_password WHERE user_ID = " + tempUser->tempID;

	chatLogLine_ = "User "
		+ tempUser->tempID
		+ " - Database: get a user password query. ";
	logger_.createLogLine(chatLogLine_);

	//queryPrint();
	if (chatDB.databaseQuery(queryString))
	{
		if (!chatDB.resultQuery_.empty())
		{
			tempUser->tempPasswordHash = chatDB.resultField_;

			chatLogLine_ = "User "
				+ tempUser->tempID
				+ " - Database: users password is found. ";
			logger_.createLogLine(chatLogLine_);
		}
		else
		{
			chatLogLine_ = "User "
				+ tempUser->tempID
				+ " - Database: users password is not found. ";
			logger_.createLogLine(chatLogLine_);

			//std::cout << "Password " << tempUser->tempLogin << " isn't found.";
			return false;
		}
	}
	else
	{
		sendDBErrorMessage();
		logger_.createLogLine(chatDB.databaseLog_.str());
	}
	return true;
}

bool Chat::searchUserByName(tempUserData* tempUser, std::string& recipient)
{
	int numberOfResultRow{ 0 };
	queryString = std::string("SELECT user_id, name, surname, ")
		+ "status, delivered_message, viewed_message "
		+ "FROM users WHERE name = '" + recipient + "'";

	chatLogLine_ = "User "
		+ loginUser_.getUserID()
		+ " - Database: search query for a user by name "
		+ recipient;
	logger_.createLogLine(chatLogLine_);

	//queryPrint();
	if (chatDB.databaseQuery(queryString))
	{
		if (chatDB.resultQuery_.empty())
		{
			chatLogLine_ = "User "
				+ loginUser_.getUserID()
				+ " - Database: User "
				+ recipient
				+ " isn't found.";
			logger_.createLogLine(chatLogLine_);
			
			//std::cout << "User " << tempUser->tempLogin << " isn't found.";
			return false;
		}
		else if (chatDB.resultQuery_.size() > 1)
		{			
			for (auto resultRow : chatDB.resultQuery_)
			{
				if (resultRow.at(3) != "0")
				{
					server.sendData_ = serverCommand::serviceMsg
						+ std::to_string(numberOfResultRow + 1)	+ "\t" 
						+ std::string(resultRow.at(1)) + " "
						+ std::string(resultRow.at(2)) + "\n";
					server.dataTransmission();
				}
				numberOfResultRow++;
			}
			server.sendData_ = textColor::yellow
				+ "Select the recipient's number: "
				+ textColor::resetColor;
			server.dataTransmission();

			chatLogLine_ = "User "
				+ loginUser_.getUserID()
				+ " - Database: User "
				+ recipient
				+ " not the only one. Sent userlist.";
			logger_.createLogLine(chatLogLine_);

			server.dataRecieving();
			int recipientNumber = std::stoi(server.message) - 1;
			if (recipientNumber < 0 || recipientNumber > numberOfResultRow)
			{
				server.sendData_ = textColor::red
					+ "Recipient not found... "
					+ textColor::resetColor;
				server.dataTransmission();

				chatLogLine_ = "User "
					+ loginUser_.getUserID()
					+ " - recipient isn't found.";
				logger_.createLogLine(chatLogLine_);

				return false;
			}
			else
			{
				numberOfResultRow = recipientNumber;
			}
		}
		tempUser->tempID =
			chatDB.resultQuery_.at(numberOfResultRow).at(0);
		tempUser->tempName =
			chatDB.resultQuery_.at(numberOfResultRow).at(1);
		tempUser->tempSurname =
			chatDB.resultQuery_.at(numberOfResultRow).at(2);
		tempUser->tempStatus =
			chatDB.resultQuery_.at(numberOfResultRow).at(3);
		tempUser->tempDeliveredMessage =
			chatDB.resultQuery_.at(numberOfResultRow).at(4);
		tempUser->tempViewedMessage =
			chatDB.resultQuery_.at(numberOfResultRow).at(5);

		chatLogLine_ = "User "
			+ loginUser_.getUserID()
			+ " - Database: User "
			+ recipient
			+ " is found.";
		logger_.createLogLine(chatLogLine_);
		//std::cout << "User " << recipient << " is found.";
	}
	else
	{
		sendDBErrorMessage();
		logger_.createLogLine(chatDB.databaseLog_.str());
	}
	return true;
}

bool Chat::checkUserFullName(tempUserData* tempUser)
{	
	queryString = std::string("SELECT user_id ")
		+ "FROM users WHERE name = '" + tempUser->tempName
		+ "' AND surname = '" + tempUser->tempSurname + "'";

	chatLogLine_ = "User "
		+ loginUser_.getUserID()
		+ " - Database: search query for a user by full name: "
		+ tempUser->tempName + " "
		+ tempUser->tempSurname;
	logger_.createLogLine(chatLogLine_);

	//queryPrint();
	if (chatDB.databaseQuery(queryString))
	{
		if (chatDB.resultQuery_.empty())
		{					
			chatLogLine_ = "User "
				+ loginUser_.getUserID()
				+ " - Database: User "
				+ tempUser->tempName + " "
				+ tempUser->tempSurname
				+ " is not found.";
			logger_.createLogLine(chatLogLine_);
			//std::cout << "... False\n";
			return false;			
		}
		tempUser->tempID = chatDB.resultField_;
		return true;
	}
	else
	{
		sendDBErrorMessage();
		logger_.createLogLine(chatDB.databaseLog_.str());
		return false;
	}		
}

void Chat::viewChat()
{	
	std::string messageID;
	std::string userID = loginUser_.getUserID();
	do
	{
		server.dataRecieving();
		if (std::string(server.message) == serverCommand::confirmationMsg)
		{
			break;
		}

		messageID = std::string(server.message);
		if (std::stoul(messageID) > std::stoul(loginUser_.getViewedMessage()))
		{
			queryString = std::string("UPDATE users ")
				+ "SET viewed_message = " + messageID				
				+ " WHERE user_id = " + userID;
			// queryPrint();
			if (!chatDB.databaseQuery(queryString))
			{
				sendDBErrorMessage();
			}
			loginUser_.setViewedMessage(messageID);

			chatLogLine_ = "User "
				+ loginUser_.getUserID()
				+ " - Database: set the last viewed message. ";				
			logger_.createLogLine(chatLogLine_);
		}		
	} while (true);	
}

std::string Chat::requestingUserParameter(std::string parameter)
{	
	std::string isPassword = 
		parameter == userParameters::password
		? serverCommand::passwordMsg : "";
	server.sendData_ = isPassword
		+ textColor::yellow
		+ parameter + ": "		
		+ textColor::resetColor;	
	server.dataTransmission();
	server.dataRecieving();
	return std::string(server.message);
}

void Chat::queryPrint()
{
	std::cout << "Query to DB: " << textColor::magenta << queryString
		<< textColor::resetColor << "\n";
}

void Chat::sendDBErrorMessage()
{
	server.sendData_ = serverCommand::exitChatMsg
		+ textColor::red
		+ "Database error. Please return later...\n"
		+ textColor::resetColor;
	server.dataTransmission();
}
