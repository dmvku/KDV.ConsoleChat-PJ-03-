#pragma once
#include <string>

class Message
{
public:
	//Message() = default;
	Message(const std::string& from, const std::string& to, const std::string& message, bool isPrivateMessage);
	~Message();

	const std::string getFrom() const;
	const std::string getTo() const;
	const std::string getMessage() const;
	const bool getIsPrivateMessage() const;

	/*void set_From(std::string from);
	void set_To(std::string to);*/
	

private:
	std::string from_{};
	std::string to_{};
	std::string message_{};
	bool isPrivateMessage_{ false };
	//bool messageIsDelete_{ false };

	void set_Message(std::string& message);
};