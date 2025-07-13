STD = -std=c++17
MAIN = TCP_Server_Linux
LIB = ChatLIB.a
LIBSQL = -L/usr/include/mysql -lmysqlclient -I/usr/include/mysql

server : $(MAIN).o $(LIB)
	g++ $(STD) -o $@ $(MAIN).o -L. lib$(LIB) $(LIBSQL)

$(MAIN).o : $(MAIN).cpp
	g++ $(STD) -o $@ $(MAIN).cpp -c

$(LIB) : chat.o user.o socket.o server_config.o DB_config.o database.o 
	ar rc lib$(LIB) chat.o user.o socket.o server_config.o DB_config.o database.o 

chat.o : chat.cpp
	g++ $(STD) -o $@ chat.cpp -c

user.o : user.cpp
	g++ $(STD) -o $@ user.cpp -c

socket.o : socket.cpp
	g++ $(STD) -o $@ socket.cpp -c

server_config.o : server_config.cpp
	g++ $(STD) -o $@ server_config.cpp -c

DB_config.o : DB_config.cpp
	g++ $(STD) -o $@ DB_config.cpp -c

database.o : database.cpp
	g++ $(STD) -o $@ database.cpp $(LIBSQL) -c 

clean:	
	rm -rf *.o *.a 

exec:
	./server
