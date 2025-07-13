#pragma once
#include <string>

class User
{
public:
	User() = default;
	User(unsigned long userID,
		const std::string& name,
		const std::string& surname);
	~User();
		
	const unsigned long getUserID() const;
	const std::string& getName() const;
	const std::string& getSurname() const;

	void setUserID(unsigned long userID);
	void setName(std::string& name);
	void setSurname(std::string& surname);

private:
	unsigned long userID_;
	std::string name_;
	std::string surname_;	
};
