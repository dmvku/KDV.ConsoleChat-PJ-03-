#include "user.h"

User::User(std::string userID) : userID_{ userID }
{

}

User::~User()
{

}

const std::string User::getUserID() const
{
	return userID_;
}

const std::string User::getDeliveredMessage() const
{
	return deliveredMessage_;
}

const std::string User::getViewedMessage() const
{
	return viewedMessage_;
}

void User::setUserID(std::string userID)
{
	userID_ = userID;
}

void User::setDeliveredMessage(std::string& deliveredMessage)
{
	deliveredMessage_ = deliveredMessage;
}

void User::setViewedMessage(std::string& viewedMessage)
{
	viewedMessage_ = viewedMessage;
}

void User::clearUser()
{
	userID_.clear();
	deliveredMessage_.clear();
	viewedMessage_.clear();
}