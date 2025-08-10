#include "logger.h"

#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <chrono>
#include <thread>
#include <shared_mutex>

Logger::Logger()
{
	std::cout << "open file...\n";
	openFile();
}

Logger::~Logger()
{
	fileStream_.close();
}

void Logger::openFile()
{
	fileStream_.open(serverLogFile,
		std::ios::in | std::ios::out | std::ios::app);
	if (!fileStream_.is_open())
	{
		std::cout << "Log file not found...\n";
		createFile();		
	}
	createlogTable();
}

void Logger::createlogTable()
{
	fileStream_.seekg(0, std::fstream::beg);
	if (fileStream_.tellg() != std::fstream::end)
	{		
		while (fileStream_.good())
		{
			logTable_.push_back(static_cast<int>(fileStream_.tellg()));
			fileStream_.ignore(maxStringLenght, '\n');
			if (fileStream_.eof())
			{
				logTable_.pop_back();
				break;
			}			
		}
	}
	
	fileStream_.seekg(0, std::fstream::end);	
}

void Logger::createLogLine(std::string logLine)
{
	logLine_ = fixingTheCurrentTime() + " " + logLine + "\n";
	std::thread saveLog(&Logger::saveLogLine, this);
	saveLog.join();
}

void Logger::saveLogLine()
{
	if (!fileStream_.is_open())
	{
		createFile();
	}
	logMutex.lock();
	fileStream_.seekg(0, std::fstream::end);
	logTable_.push_back(static_cast<int>(fileStream_.tellg()));
	fileStream_ << logLine_;
	logMutex.unlock();
}

void Logger::readingLog()
{	
	if (!logTable_.empty())
	{
		std::cout << "For view log press: p - previous, "
			<< "n - next, other key - exit.\n";
		std::thread readLog(&Logger::readLogLine, this);
		readLog.join();
	}	
	else
	{
		std::cout << "Log file is empty...\n";
	}
}

void Logger::readLogLine()
{
	std::string dataLine;
	size_t logLineNumber{ logTable_.size() - 1 };
	
	char action;
	while (true)
	{
		std::cin >> action;
		if (action != 'p' && action != 'n')
		{
			break;
		}

		logMutex.lock_shared();		
		if (action == 'p')
		{			
			if (logLineNumber != 0)
			{
				logLineNumber--;
			}
		}
		else if (action == 'n')
		{			
			if (logLineNumber < logTable_.size() - 1)
			{
				logLineNumber++;
			}
		}
		fileStream_.seekg(logTable_.at(logLineNumber), std::fstream::beg);
		std::getline(fileStream_, dataLine);
		std::cout << "Log entry " << logLineNumber << ": " << dataLine << "\n";
		fileStream_.tellg();
		logMutex.unlock_shared();
	}
}

void Logger::createFile()
{
	std::fstream fileStream(serverLogFile,
		std::ios::in | std::ios::out | std::ios::app);
	std::filesystem::permissions(serverLogFile,
		std::filesystem::perms::group_all | std::filesystem::perms::others_all,
		std::filesystem::perm_options::remove);
}

std::string Logger::fixingTheCurrentTime()
{
	std::time_t now = std::chrono::system_clock::to_time_t(
		std::chrono::system_clock::now());

	std::string currentTime(20, '\0');
	std::strftime(&currentTime[0], currentTime.size(),
		"%Y-%m-%d %H:%M:%S", std::localtime(&now));
	return currentTime;
}
