#include "user_class.h"

User::User(const std::string& login, const std::string& password, const std::string& name)
	: login_{ login }, password_{ password }, name_{ name } {}

User::~User()
{
	
}

const std::string& User::getLogin() const
{
	return login_;
}

const std::string& User::getPassword() const
{
	return password_;
}

const std::string& User::getName() const
{
	return name_;
}

void User::setPassword(std::string& password)
{
	password_ = password;
}

void User::setName(std::string& name)
{
	name_ = name;
}
	