#pragma once
#include <string>

class Message
{
public:
	//Message() = default;
	Message(const std::string& from, const std::string& to, const std::string& message);
	~Message();

	const std::string get_From() const;
	const std::string get_To() const;
	const std::string get_Message() const;

	/*void set_From(std::string from);
	void set_To(std::string to);*/
	

private:
	std::string from_{};
	std::string to_{};
	std::string message_{};
	//bool messageIsDelete_{ false };

	void set_Message(std::string& message);
};