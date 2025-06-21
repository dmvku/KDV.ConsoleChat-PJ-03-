#include "message.h"

Message::Message(const std::string& from, const std::string& to,
	const std::string& message)	:
	from_{ from },
	to_{ to },
	message_{ message }
{

}

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

void Message::set_Message(std::string& message)
{
	message_ = message;
}
