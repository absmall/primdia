#include <Exception.h>

unset_parameter::unset_parameter(const std::string &s) : std::logic_error(s)
{
}

cancelled::cancelled(const std::string &s) : std::logic_error(s)
{
}
