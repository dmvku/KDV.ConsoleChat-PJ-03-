#include "exception.h"

Warning::Warning() {}

Warning::~Warning() {}

const char* Warning::what() const noexcept
{
    return "WARNING:  ";
}
