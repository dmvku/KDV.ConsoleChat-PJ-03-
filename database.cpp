#include "database.h"
#include "database_constructor.h"

#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <unistd.h>

Database::Database()
{
	connectionToDBServer();
}

Database::~Database()
{
	mysql_close(&mysql);
}

void Database::connectionToDBServer()
{
	// Получаем дескриптор соединения
	mysql_init(&mysql);
	if (&mysql == nullptr)
	{
		// Если дескриптор не получен — выводим сообщение об ошибке
		std::cout << logColor::red
			<< "Error: can't create MySQL-descriptor\n"
			<< logColor::resetColor;
	}

	// Подключаемся к серверу
	if (!mysql_real_connect(&mysql, configDB.getHostDB().c_str(),
		configDB.getUserDB().c_str(), configDB.getPasswordDB().c_str(),
		NULL, configDB.getPortDB(),	NULL, 0))
	{
		// Если нет соединения с сервером БД выводим сообщение об ошибке
		std::cout << logColor::red 
			<< "Error: can't connect to database server.\n"
			<< mysql_error(&mysql) << logColor::resetColor << "\n";
		mysqlError = mysql_errno(&mysql);
		return;
	}
	else
	{
		// Если соединение успешно установлено выводим фразу — "Success!"
		std::cout << logColor::green 
			<< "Success! Connected to the server!\n" << logColor::resetColor;
	}

	std::cout << logColor::yellow << "Search for the '"
		<< configDB.getNameDB() << "' database...\n"
		<< logColor::resetColor;
	std::string preparingQuery = std::string("SELECT COUNT(SCHEMA_NAME) ")
		+ "FROM INFORMATION_SCHEMA.SCHEMATA "
		+ "WHERE SCHEMA_NAME = '" + configDB.getNameDB() + "'";
	if (databaseQuery(preparingQuery))
	{
		if (resultField_ == "0")
		{
			std::cout << logColor::yellow << "The database '"
				<< configDB.getNameDB()	<< "' is not found...\n"
				<< logColor::resetColor;
		}
		else
		{
			std::cout << logColor::green << "The database '"
				<< configDB.getNameDB()	<< "' is found...\033[0m\n"
				<< logColor::resetColor;
			std::cout << "What do you want to do with the database '"
				<< configDB.getNameDB()
				<< "' ('d' - delete, other key - use)? ";
			char action{ '\0' };
			std::cin >> action;
			if (action == 'd')
			{
				preparingQuery = std::string("DROP DATABASE ")
					+ configDB.getNameDB();	
				if (databaseQuery(preparingQuery))
				{
					std::cout << logColor::red << "The database '"
						<< configDB.getNameDB()	<< "' has been deleted...\n"
						<< logColor::resetColor;
				}
			}
		}
	}
	
	connectedStatusDB = mysql_select_db(&mysql, configDB.getNameDB().c_str());
	if (!connectedStatusDB)
	{
		std::cout << logColor::green << "Success! Connected to database: "
			<< configDB.getNameDB() << logColor::resetColor << "\n";
		return;
	}
	else
	{
		creatingDB();
	}

	mysql_set_character_set(&mysql, "utf8");
	//Смотрим изменилась ли кодировка на нужную, по умолчанию идёт latin1
	std::cout << "connection characterset: "
		<< mysql_character_set_name(&mysql) << "\n";
}

bool Database::databaseQuery(std::string& query)
{	
	resultQuery_.clear();
	bzero(query_content, QUERY_LENGTH);
	std::copy(query.begin(), query.end(), query_content);
	
	for (int attemptCount = 1; attemptCount <= 3; attemptCount++)
	{
		std::cout << attemptCount << " - attempt... ";
		if (mysql_query(&mysql, query_content)) //Делаем запрос к таблице
		{
			std::cout << logColor::red << "Error: " << logColor::resetColor
				<< mysql_errno(&mysql) << " " << mysql_error(&mysql) << "\n";
			mysqlError = mysql_errno(&mysql);
			sleep(3); 			
		}
		else
		{
			std::cout << logColor::green << "Done!"
				<< logColor::resetColor << "\n";

			//Выводим все что есть в базе через цикл
			if (result_ = mysql_store_result(&mysql))
			{
				while (row_ = mysql_fetch_row(result_))
				{
					resultRow_.clear();
					for (int fieldCount = 0; fieldCount < mysql_num_fields(result_); fieldCount++)
					{
						resultField_.clear();
						std::ostringstream field;
						field << row_[fieldCount];
						resultField_ = field.str();
						resultRow_.push_back(resultField_);						
					}
					resultQuery_.push_back(resultRow_);									
				}
				mysql_free_result(result_);
				return true;
			}
			else
			{
				std::cout << "The query returned NULL.\n";
				mysqlError = mysql_errno(&mysql);
				if (!mysqlError)
				{
					return true;
				}
				std::cout << logColor::red << "Error: "
					<< logColor::resetColor << mysql_errno(&mysql)
					<< " " << mysql_error(&mysql) << "\n";				
			}
		}		
	}	
	return false;
}

void Database::creatingDB()
{
	std::cout << logColor::yellow << "Creating DB... "
		<< logColor::resetColor;
	databaseQuery(createDB);
	
	connectedStatusDB = mysql_select_db(&mysql, configDB.getNameDB().c_str());
	if (connectedStatusDB)
	{
		std::cout << logColor::red << "Error: " << logColor::resetColor
			<< mysql_errno(&mysql) << " " << mysql_error(&mysql)
			<< "\nCheck database parameters...\n";
		mysqlError = mysql_errno(&mysql);
		return;
	}
	else
	{
		std::cout << logColor::green << "Success! Connected to database: "
			<< configDB.getNameDB() << logColor::resetColor << "\n";
	}	
	
	std::cout << logColor::yellow << "Creating users table... "
		<< logColor::resetColor;
	databaseQuery(createUsersTable);

	std::cout << logColor::yellow << "Creating password hash table... "
		<< logColor::resetColor;
	databaseQuery(createUsersPasswordTable);

	std::cout << logColor::yellow << "Creating messages table... "
		<< logColor::resetColor;
	databaseQuery(createMessagesTable);

	std::cout << logColor::yellow << "Creating users view... "
		<< logColor::resetColor;
	databaseQuery(createUserListView);

	std::cout << logColor::yellow << "Creating chat view ... "
		<< logColor::resetColor;
	databaseQuery(createChatListView);

	/*std::cout << "Creating search user by login function... ";
	databaseQuery(createSearchUserByLoginFunction);

	std::cout << "Creating search user by name function... ";
	databaseQuery(createSearchUserByNameFunction);
		
	std::cout << "Creating login user procedure... ";
	databaseQuery(createloginUserProcedure);

	std::cout << "Creating recieve user data by login procedure... ";
	databaseQuery(createRecieveUserProcedure);

	std::cout << "Creating recieve user password by ID procedure... ";
	databaseQuery(createRecievePasswordProcedure);

	std::cout << "Creating add user procedure... ";
	databaseQuery(createAddUserProcedure);

	std::cout << "Creating delete user procedure... ";
	databaseQuery(createDeleteUserProcedure);

	std::cout << "Creating add messages procedure... ";
	databaseQuery(createAddMessageProcedure);

	std::cout << "Creating delete message procedure... ";
	databaseQuery(createSetDeletedMessageProcedure);

	std::cout << "Creating view chat procedure... ";
	databaseQuery(createViewChatProcedure);
		
	std::cout << "Creating set message is viewed procedure... ";
	databaseQuery(createSetViewedMessageProcedure);*/

	std::cout << logColor::yellow << "Adding account 'to_all'... "
		<< logColor::resetColor;
	databaseQuery(addToAllAccount);

	std::cout << logColor::yellow << "Adding password for 'to_all'... "
		<< logColor::resetColor;
	databaseQuery(addToAllPassword);	
}
