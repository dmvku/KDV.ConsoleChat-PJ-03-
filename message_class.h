#pragma once
#include <string>

class Message
{
public:
	Message(const std::string& from, const std::string& to, const std::string& message);
	~Message();

	const std::string getFrom() const;
	const std::string getTo() const;
	const std::string getMessage() const;	

private:
	std::string from_;
	std::string to_;
	std::string message_;
};
