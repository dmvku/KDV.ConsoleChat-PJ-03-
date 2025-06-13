#include "user_class.h"

User::User(const std::string& login, const std::string& passwordHash,
	const std::string& name)
	: login_{ login }, passwordHash_{ passwordHash }, name_{ name } {
}

User::~User()
{

}

const std::string& User::getLogin() const
{
	return login_;
}

const std::string& User::getPasswordHash() const
{
	return passwordHash_;
}

const std::string& User::getName() const
{
	return name_;
}

void User::setLogin(std::string& login)
{
	login_ = login;
}

void User::setPasswordHash(std::string& passwordHash)
{
	passwordHash_ = passwordHash;
}

void User::setName(std::string& name)
{
	name_ = name;
}
