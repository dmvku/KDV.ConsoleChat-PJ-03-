#include "exception_class.h"

Warning::Warning() {}

Warning::~Warning() {}

const char* Warning::what() const noexcept
{
    return "WARNING:  ";
}
