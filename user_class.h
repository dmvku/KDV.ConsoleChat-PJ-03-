#pragma once
#include <string>
#include <list>

#include "sha1.h"

class User
{
public:
	User();
	User(const std::string& login, uint* pass_sha1_hash, const std::string& name);
	~User();

	User& operator = (const User& other);
	bool operator == (const User& other);
		
	const std::string& getLogin() const;
	const uint* getPasswordHash() const;
	const std::string& getName() const;

	//void setPassword(std::string& password);
	void setName(std::string& name);

private:
		
	std::string login_{};
	uint* pass_sha1_hash;
	std::string name_{};

};
