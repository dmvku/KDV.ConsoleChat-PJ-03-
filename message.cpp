#include "message.h"

Message::Message(unsigned long messageID,
	unsigned long from,
	unsigned long to,
	std::string message,
	std::string createTime)	:
	messageID_{ messageID },
	from_{ from },
	to_{ to },
	message_{ message },
	createTime_{ createTime }
{

}

Message::~Message()
{

}


const unsigned long Message::getMessageID() const
{
	return messageID_;
}

const unsigned long Message::getFrom() const
{
	return from_;
}

const unsigned long Message::getTo() const
{
	return to_;
}

const std::string Message::getMessage() const
{
	return message_;
}

const std::string Message::getCreateTime() const
{
	return createTime_;
}

void Message::setMessageID(unsigned long messageID)
{
	messageID_ = messageID;
}

void Message::setFrom(unsigned long from)
{
	from_ = from;
}

void Message::setTo(unsigned long to)
{
	to_ = to;
}

void Message::setMessage(std::string message)
{
	message_ = message;
}

void Message::setCreateTime(std::string createTime)
{
	createTime_ = createTime;
}
