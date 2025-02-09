#pragma once
#include <string>

class User
{
public:
	User() = default;
	User(const std::string& login, const std::string& password, const std::string& name);
	~User();
		
	const std::string& getLogin() const;
	const std::string& getPassword() const;
	const std::string& getName() const;

	void setPassword(std::string& password);
	void setName(std::string& name);

private:
	const std::string login_{};
	std::string password_{};
	std::string name_{};

};