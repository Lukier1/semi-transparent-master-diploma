#pragma once

#include <exception>
#include <string>
class UnknownImageFormat : public std::exception {
public:
	UnknownImageFormat(std::string message) : msg(("Uknown image format " + message).c_str()) {
	}
protected:
	virtual const char * what() const throw() {
		return msg;
	}
	const char * msg;
};