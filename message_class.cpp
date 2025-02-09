#include "message_class.h"

Message::Message(const std::string& from, const std::string& to, const std::string& message,
	const std::string creationTime, bool isPrivateMessage)
	: from_{ from }, to_{ to }, message_{ message }, creationTime_(creationTime), 
	isPrivateMessage_(isPrivateMessage) {}

Message::~Message()
{

}

const std::string Message::getFrom() const
{
	return from_;
}

const std::string Message::getTo() const
{
	return to_;
}

const std::string Message::getMessage() const
{
	return message_;
}

const std::string Message::getCreationTime() const
{
	return creationTime_;
}

const bool Message::getIsPrivateMessage() const
{
	return isPrivateMessage_;
}

void Message::setMessage(std::string& message)
{
	message_ = message;
}
