#pragma once

class Logger {
public:
	static Logger * getLogger();
	void Error(std::string description);
	void Info(std::string descrption);
	bool gl_log(int error, const char* description);
	~Logger();

private:
	std::fstream file;
	const std::string filename = "gl.log";
	Logger();
	static Logger * p_Logger;

	void Write(std::string text);
};

