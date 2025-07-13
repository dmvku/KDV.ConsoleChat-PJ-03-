#pragma once
#include <string>

class User
{
public:
	User() = default;
	User(std::string userID);
	~User();

	const std::string getUserID() const;
	const std::string getDeliveredMessage() const;
	const std::string getViewedMessage() const;	
	
	void setUserID(std::string userID);
	void setDeliveredMessage(std::string& deliveredMessage);
	void setViewedMessage(std::string& viewedMessage);
		
	void clearUser();

private:
	std::string userID_{ 0 };
	std::string deliveredMessage_;	
	std::string viewedMessage_;	
};
