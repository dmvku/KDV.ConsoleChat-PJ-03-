#pragma once
#include <string>

class Message
{
public:
	Message(unsigned long messageID,
	unsigned long from,
	unsigned long to,
	std::string message,
	std::string createTime);

	~Message();

	const unsigned long getMessageID() const;
	const unsigned long getFrom() const;
	const unsigned long getTo() const;
	const std::string getMessage() const;
	const std::string getCreateTime() const;

	void setMessageID(unsigned long messageID);
	void setFrom(unsigned long from);
	void setTo(unsigned long to);
	void setMessage(std::string message);
	void setCreateTime(std::string createTime);

private:
	unsigned long messageID_{ 0 };
	unsigned long from_;
	unsigned long to_;
	std::string message_;	
	std::string createTime_;
};
