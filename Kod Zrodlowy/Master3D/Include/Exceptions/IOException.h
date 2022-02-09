#pragma once

#include <exception>
#include <string>
class IOException : public std::exception {
public:
	IOException(std::string _msg) :  msg(_msg.c_str()) {
	}
	virtual const char * what() const throw() {
		return msg;
	}
private:
	const char * msg;
};