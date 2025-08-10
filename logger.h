#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <limits>
#include <sstream>
#include <shared_mutex>

extern const char* serverLogFile;
constexpr auto maxStringLenght = std::numeric_limits<std::streamsize>::max();

class Logger
{
public:
	Logger();
	~Logger();

	void createLogLine(std::string logLine);
	void readingLog();	

private:
	std::fstream fileStream_;
	std::vector<int> logTable_;
	std::string logLine_;
	std::string timeAddLog;
	std::shared_mutex logMutex;	
		
	void openFile();
	void createlogTable();	
	void createFile();
	void saveLogLine();
	void readLogLine();
	std::string fixingTheCurrentTime();
};
