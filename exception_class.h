#pragma once
#include <exception>

class Warning : public std::exception
{
public:
    Warning();
    ~Warning();

    const char* what() const noexcept;
};
