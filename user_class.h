#pragma once
#include <string>

class User
{
public:
	User() = default;
	User(const std::string& name);
	~User();
		
	const std::string& getName() const;			
	void setName(std::string& name);	

private:	
	std::string name_;
	int UserID{};
};
