#include "exception_class.h"

Warning::Warning() {}

Warning::~Warning() {}

const char* Warning::what() const noexcept
{
    return "\033[1;33;44mWARNING:  ";
}
