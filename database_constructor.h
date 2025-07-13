#pragma once
#include <string>

std::string createDB = std::string("CREATE DATABASE IF NOT EXISTS chat");

std::string createUsersTable = std::string("CREATE TABLE users (")
+ "user_id INT UNSIGNED AUTO_INCREMENT, "
+ "login VARCHAR(50) NOT NULL, "
+ "name VARCHAR(50) NOT NULL, "
+ "surname VARCHAR(50) NOT NULL, "
+ "email VARCHAR(50), "
+ "status INT DEFAULT 1, "
+ "delivered_message BIGINT UNSIGNED DEFAULT 0, "
+ "viewed_message BIGINT UNSIGNED DEFAULT 0, "
+ "CONSTRAINT users_pk PRIMARY KEY (user_id), "
+ "CONSTRAINT users_login_unique UNIQUE (login), "+ 
+ "CONSTRAINT user_name_unique UNIQUE (name, surname))";

std::string createUsersPasswordTable = std::string("CREATE TABLE users_password (")
+ "user_id INT UNSIGNED, "
+ "password_hash VARCHAR(40) NOT NULL, "
+ "CONSTRAINT users_password_pk PRIMARY KEY (user_id), "
+ "CONSTRAINT user_fk FOREIGN KEY (user_id) "
+ "REFERENCES users(user_id) ON UPDATE CASCADE ON DELETE CASCADE)";

std::string createMessagesTable = std::string("CREATE TABLE messages (")
+ "message_id BIGINT UNSIGNED AUTO_INCREMENT, "
+ "sender_id INT UNSIGNED, "
+ "recipient_id INT UNSIGNED, "
+ "message TEXT NOT NULL, "
+ "time_create TIMESTAMP DEFAULT CURRENT_TIMESTAMP(), "
+ "status INT default 1, "
+ "CONSTRAINT messages_pk PRIMARY KEY (message_id), "
+ "CONSTRAINT sender_fk FOREIGN KEY (sender_id) REFERENCES users(user_id), "
+ "CONSTRAINT recipient_fk FOREIGN KEY (recipient_id) "
+ "REFERENCES users(user_id))";

std::string createUserListView = std::string("CREATE VIEW ")
+ "view_user_list AS "
+ "SELECT user_id, name, surname "
+ "FROM users WHERE status != 0 AND user_id != 1";

std::string createChatListView = std::string("CREATE VIEW ")
+ "chat_list AS "
+ "SELECT message_id, sender_id, recipient_id, message, time_create "
+ "FROM messages WHERE status > 0 ORDER BY time_create ASC";

std::string addToAllAccount = std::string("INSERT INTO users ")
+ "(login, name, surname, email) "
+ "VALUES ('to_all', 'to_all', 'to_all', '')";

std::string addToAllPassword = std::string("INSERT INTO users_password ")
+ "(user_id, password_hash) VALUES (1, 'to_all')";

// full userlist
//std::string createUserListFullView = std::string("CREATE VIEW ")
//+ "view_user_list_full AS "
//+ "SELECT user.user_id, name, surname, password_hash, "
//+ "delivered_message, viewed_messege, status "
//+ "FROM users "
//+ "INNER JOIN password_hash ON users_password.user_id = users.user_id "
//+ "WHERE user_id != 1";

//std::string createSearchUserByLoginFunction = std::string("CREATE FUNCTION ")
//+ "search_login_user (in_login VARCHAR(50)) "
//+ "RETURNS INT UNSIGNED "
//+ "DETERMINISTIC "
//+ "BEGIN "
//+ "RETURN "
//+ "(SELECT user_id FROM users WHERE login = in_login)\x3B"
//+ "END";

//std::string createSearchUserByNameFunction = std::string("CREATE FUNCTION ")
//+ "search_name_user (in_name VARCHAR(50), in_surname VARCHAR(50))"
//+ "RETURNS INT UNSIGNED "
//+ "DETERMINISTIC "
//+ "BEGIN "
//+ "RETURN "
//+ "(SELECT user_id FROM users WHERE name = in_name "
//+ "AND surname = in_surname)\x3B"
//+ "END";

//std::string createloginUserProcedure = std::string("CREATE PROCEDURE ")
//+ "login_user (IN in_login VARCHAR(50)) "
//+ "BEGIN "
//+ "SELECT "
//+ "users.user_id, login, name, surname,	password_hash "
//+ "FROM users_password "
//+ "INNER JOIN "
//+ "users ON users.user_id = users_password.user_id "
//+ "WHERE login = in_login\x3B"
//+ "END";

//std::string createRecieveUserProcedure = std::string("CREATE PROCEDURE ")
//+ "recieve_user (IN in_login VARCHAR(50)) "
//+ "BEGIN "
//+ "SELECT user_id, name, surname "
//+ "FROM users "
//+ "WHERE login = in_login\x3B"
//+ "END";
//
//std::string createRecievePasswordProcedure = std::string("CREATE PROCEDURE ")
//+ "recieve_password (IN in_user_id INT UNSIGNED) "
//+ "BEGIN "
//+ "SELECT password_hash "
//+ "FROM users_password "
//+ "WHERE user_id = in_user_id\x3B"
//+ "END";

//std::string createAddUserProcedure = std::string("CREATE PROCEDURE ")
//+ "add_user ("
//+ "IN in_login VARCHAR(50), "
//+ "IN in_name VARCHAR(50), "
//+ "IN in_surname VARCHAR(50), "
//+ "IN in_email VARCHAR(50), "
//+ "IN in_passwordhash VARCHAR(40)) "
//+ "BEGIN "
//+ "START TRANSACTION\x3B "
//+ "INSERT INTO "
//+ "users (login, name, surname, email) "
//+ "VALUES (in_login, in_name, in_surname, in_email)\x3B "
//+ "INSERT INTO "
//+ "users_password (user_id, password_hash) "
//+ "VALUES ( "
//+ "(SELECT user_id FROM users WHERE login = in_login), "
//+ "in_passwordhash)\x3B"
//+ "COMMIT\x3B"
//+ "END";

//std::string createDeleteUserProcedure = std::string("CREATE PROCEDURE ")
//+ "delete_user (IN in_user_id INT UNSIGNED) "
//+ "BEGIN "
//+ "START TRANSACTION\x3B"
//+ "UPDATE users SET status = 0 WHERE user_id = in_user_id\x3B"
//+ "UPDATE users_password SET passwordhash = '0' WHERE user_id = in_user_id\x3B"
//+ "COMMIT\x3B"
//+ "END";

//std::string createAddMessageProcedure = std::string("CREATE PROCEDURE ")
//+ "add_message ("
//+ "in_sender_id INT UNSIGNED, "
//+ "in_recipient_id INT UNSIGNED, "
//+ "in_message TEXT) "
//+ "BEGIN "
//+ "INSERT INTO messages (sender_id, recipient_id, message) "
//+ "VALUE (in_sender_id, in_recipient_id, in_message)\x3B"
//+ "END";

//std::string createSetDeletedMessageProcedure = std::string("CREATE PROCEDURE ")
//+ "set_deleted_message (in_message_id BIGINT UNSIGNED) "
//+ "BEGIN "
//+ "UPDATE messages SET status = -1 "
//+ "WHERE message_id = in_message_id\x3B"
//+ "END";

//std::string createViewChatProcedure = std::string("CREATE PROCEDURE ")
//+ "send_chat (in_user_id INT UNSIGNED) "
//+ "BEGIN "
//+ "START TRANSACTION\x3B"
//+ "UPDATE users SET delivered_message = ("
//+ "SELECT message_id FROM chat_list WHERE time_create = ("
//+ "SELECT max(time_create) FROM chat_list "
//+ "WHERE sender_id = in_user_id "
//+ "OR recipient_id = in_user_id "
//+ "OR recipient_id = 1))\x3B"
//+ "SELECT * FROM chat_list "
//+ "WHERE sender_id = in_user_id "
//+ "OR recipient_id = in_user_id "
//+ "OR recipient_id = 1 "
//+ "ORDER BY time_create ASC\x3B"
//+ "COMMIT\x3B"
//+ "END";

//std::string createSetViewedMessageProcedure = std::string("CREATE PROCEDURE ")
//+ "set_viewed_message (in_message_id BIGINT UNSIGNED) "
//+ "BEGIN "
//+ "UPDATE users "
//+ "SET viweded_message = IF("
//+ "viweded_message < in_message_id, in_message_id, viweded_message)\x3B"
//+ "END";