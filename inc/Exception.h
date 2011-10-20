#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <config/common.h>

#include <string>
#include <stdexcept>

class unset_parameter : std::logic_error
{
public:
	unset_parameter(const std::string &s);
};

class cancelled : std::logic_error
{
public:
	cancelled(const std::string &s);
};

#endif /* __EXCEPTION_H__ */
