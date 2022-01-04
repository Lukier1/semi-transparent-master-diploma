#include "stdafx.h"

#include <Logger.h>
#include<Windows.h>
Logger::Logger() {
	file.open(filename, std::fstream::out | std::fstream::trunc);
	if (!file.is_open())
	{
		std::cout << "Can't open file " << filename;
	}
	else
	{
		file << "Log started" << std::endl;
		file.close();
	}
}
	
Logger * Logger::getLogger()
{
	if (!p_Logger)
		p_Logger = new Logger();
	return p_Logger;
}
void Logger::Error(std::string description)
{
	MessageBox(nullptr, ("Error:\t" + description).c_str(), "ERROR", MB_OK | MB_ICONERROR);
	Write("Error:\t" + description);
}


void Logger::Info(std::string text) {
	Write("Info:\t" + text);
}

bool Logger::gl_log(int error, const char* description)
{
	file.open(filename, std::fstream::out | std::fstream::app);
	if (!file.is_open())
	{
		std::cout << "Can't open file " << filename;
		return false;
	}
	file << "GLWF Error code: " << error << " msg " << description << std::endl;
	std::cout << "GLWF Error code: " << error << " msg " << description << std::endl;
	file.close();
	return true;
}
Logger::~Logger() {

}


void Logger::Write(std::string text)
{
	file.open(filename, std::fstream::out | std::fstream::app);
	if (!file.is_open())
	{
		std::cout << "Can't open file " << filename;
	}
	file << text << std::endl;
	std::cout << text << std::endl;
	file.close();
}
Logger * Logger::p_Logger = nullptr;