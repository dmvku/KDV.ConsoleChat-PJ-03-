STD = -std=c++17
MAIN = TCP_Client_Linux
LIB = ChatLIB.a

client : $(MAIN).o $(LIB)
	g++ $(STD) -o $@ $(MAIN).o -L. lib$(LIB)

$(MAIN).o : $(MAIN).cpp
	g++ $(STD) -o $@ $(MAIN).cpp -c

$(LIB) : chat.o message.o user.o sha1.o socket.o connection_config.o
	ar rc lib$(LIB) chat.o message.o user.o sha1.o socket.o connection_config.o

chat.o : chat.cpp
	g++ $(STD) -o $@ chat.cpp -c

message.o : message.cpp
	g++ $(STD) -o $@ message.cpp -c

user.o : user.cpp
	g++ $(STD) -o $@ user.cpp -c

sha1.o : sha1.cpp
	g++ $(STD) -o $@ sha1.cpp -c

socket.o : socket.cpp
	g++ $(STD) -o $@ socket.cpp -c

connection_config.o : connection_config.cpp
	g++ $(STD) -o $@ connection_config.cpp -c

clean:	
	rm -rf *.o *.a 

exec:
	./server
