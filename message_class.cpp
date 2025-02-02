#include "message_class.h"

Message::Message(const std::string& from, const std::string& to, const std::string& message)
	: from_{ from }, to_{ to }, message_{ message } {}

Message::~Message()
{

}

const std::string Message::get_From() const
{
	return from_;
}

const std::string Message::get_To() const
{
	return to_;
}

const std::string Message::get_Message() const
{
	return message_;
}

/*void set_From(std::string from)
void set_To(std::string to)*/

void Message::set_Message(std::string& message)
{
	message_ = message;
}
