#include "user.h"

User::User(const std::string& name)	: name_{ name }
{

}

User::~User()
{

}

const std::string& User::getName() const
{
	return name_;
}

void User::setName(std::string& name)
{
	name_ = name;
}
