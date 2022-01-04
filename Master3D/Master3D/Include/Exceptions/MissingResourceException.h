#pragma once
#include <string>
#include <exception>

class MissingResourceException : public std::exception {
private:
	std::string data;
public:
	MissingResourceException(std::string type, std::string filename) {
		data = "Can't load " + type + " from file " + filename + "\n";
	}
	const char * what() {
		return data.c_str();
	};
};