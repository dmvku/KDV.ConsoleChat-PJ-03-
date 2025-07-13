#pragma once
#include "DB_config.h"

#include <iostream>
#include <string>
#include <vector>
#include "/usr/include/mysql/mysql.h"

#define QUERY_LENGTH 2048

namespace logColor
{
	const std::string resetColor = "\033[0m";
	const std::string red = "\033[31m";
	const std::string green = "\033[32m";
	const std::string yellow = "\033[33m";
}

class Database
{
public:
	MYSQL_RES* result_;
	MYSQL_ROW row_;
	std::string resultField_;
	std::vector <std::string> resultRow_;
	std::vector <std::vector <std::string>> resultQuery_;

	bool databaseQuery(std::string& query);
	unsigned int mysqlError{ 0 };	

	Database();
	~Database();	

private:
	DBConfig configDB;
	MYSQL mysql;

	int connectedStatusDB{};
	char query_content[QUERY_LENGTH];

	void connectionToDBServer();
	void creatingDB();	
};