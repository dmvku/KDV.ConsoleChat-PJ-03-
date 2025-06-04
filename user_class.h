#pragma once
#include <string>

class User
{
public:
	User() = default;
	User(const std::string& login, const std::string& passwordHash, const std::string& name);	
	~User();
		
	const std::string& getLogin() const;
	const std::string& getPasswordHash() const;
	const std::string& getName() const;

	void setLogin(std::string& login);
	void setPasswordHash(std::string& passwordHash);
	void setName(std::string& name);

private:
	std::string login_{};
	std::string passwordHash_{};
	std::string name_{};

};