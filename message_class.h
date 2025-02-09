#pragma once
#include <string>

class Message
{
public:
	Message(const std::string& from, const std::string& to, const std::string& message,
		const std::string creationTime, bool isPrivateMessage);
	~Message();

	const std::string getFrom() const;
	const std::string getTo() const;
	const std::string getMessage() const;
	const std::string getCreationTime() const;
	const bool getIsPrivateMessage() const;
	
private:
	std::string from_{};
	std::string to_{};
	std::string message_{};
	std::string creationTime_{};
	bool isPrivateMessage_{ false };
	
	void setMessage(std::string& message);
};