#include "user_class.h"

User::User() : login_(""), pass_sha1_hash(0), name_("") {}

User::User(const std::string& login, uint* sh1, const std::string& name)
	: login_{ login }, pass_sha1_hash{ sh1 }, name_{ name } {}

User::~User()
{
	if (pass_sha1_hash != 0)
	{
		delete[] pass_sha1_hash;
	}
}

User& User::operator = (const User& other)
{
	login_ = other.login_;
	name_ = other.name_;

	if (pass_sha1_hash != 0)
	{
		delete[] pass_sha1_hash;
	}

	pass_sha1_hash = new uint[SHA1HASHLENGTHUINTS];
	memcpy(pass_sha1_hash, other.pass_sha1_hash, SHA1HASHLENGTHBYTES);

	return *this;
}

bool User::operator == (const User& other)
{
	return  pass_sha1_hash == other.pass_sha1_hash
		&& login_ == other.login_;
}

const std::string& User::getLogin() const
{
	return login_;
}

const uint* User::getPasswordHash() const
{
	return pass_sha1_hash;
}

const std::string& User::getName() const
{
	return name_;
}

//void User::setPassword(std::string& password)
//{
//	password_ = password;
//}

void User::setName(std::string& name)
{
	name_ = name;
}
	