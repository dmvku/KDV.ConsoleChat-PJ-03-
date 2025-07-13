#include "user.h"

User::User(unsigned long userID,
	const std::string& name,
	const std::string& surname) :
	userID_{ userID },
	name_{ name },
	surname_{ surname }
{

}

User::~User()
{

}

const unsigned long User::getUserID() const
{
	return userID_;
}

const std::string& User::getName() const
{
	return name_;
}

const std::string& User::getSurname() const
{
	return surname_;
}

void User::setUserID(unsigned long userID)
{
	userID_ = userID;
}

void User::setName(std::string& name)
{
	name_ = name;
}

void User::setSurname(std::string& surname)
{
	surname_ = surname;
}
