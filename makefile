STD = -std=c++17
PFX = _class.o
MAIN = TCP_Server_Linux
LIB = ChatLIB.a

server : $(MAIN).o $(LIB)
	g++ $(STD) -o $@ $(MAIN).o -L. lib$(LIB)

$(MAIN).o : $(MAIN).cpp
	g++ $(STD) -o $@ $(MAIN).cpp -c

$(LIB) : chat$(PFX) message$(PFX) user$(PFX) exception$(PFX) connection_config.o
	ar rc lib$(LIB) chat$(PFX) message$(PFX) user$(PFX) exception$(PFX) connection_config.o

chat$(PFX) : chat$(PFX:.o=.cpp)
	g++ $(STD) -o $@ chat$(PFX:.o=.cpp) -c

message$(PFX) : message$(PFX:.o=.cpp)
	g++ $(STD) -o $@ message$(PFX:.o=.cpp) -c

user$(PFX) : user$(PFX:.o=.cpp)
	g++ $(STD) -o $@ user$(PFX:.o=.cpp) -c

exception$(PFX) : exception$(PFX:.o=.cpp)
	g++ $(STD) -o $@ exception$(PFX:.o=.cpp) -c

connection_config.o : connection_config.cpp
	g++ $(STD) -o $@ connection_config.cpp -c

clean:	
	rm -rf *.o *.a 

exec:
	./server
