#pragma once

#include <string>
#include <fstream>

#define DBPORT 3306

extern const char* DBConfigFile;

namespace parametersName
{
	const std::string n_host = "Host";
	const std::string n_user = "User";
	const std::string n_password = "Password";
	const std::string n_database = "Database";
	const std::string n_port = "Port";	
}

class DBConfig
{
public:
	DBConfig();
	~DBConfig();

	const std::string getHostDB() const;	
	const std::string getUserDB() const;
	const std::string getPasswordDB() const;
	const std::string getNameDB() const;
	const int getPortDB() const;
	
private:
	std::fstream fileStream;
	std::string hostDB_{ "localhost" };
	std::string userDB_{ "root" };
	std::string passwordDB_{ "root" };	
	std::string nameDB_;
	unsigned int portDB_{ 0 };	

	void fileOptions();
	void openFile();
	void readFile();		
	void checkParameters();
	void createFile();
	void saveFile();
	
	void checkPortDB();
	void checkHostDB();
	bool parsingAddressIP();

	void setDBConnectionParameters();
	void setHostDB();
	void setUserDB();
	void setPasswordDB();
	void setNameDB();
	void setPortDB();	
};
