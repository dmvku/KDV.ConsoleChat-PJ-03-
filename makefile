STD = -std=c++17
PFX = _class.o
MAIN = TCP_Client_Linux
LIB = ChatLIB.a

client : $(MAIN).o $(LIB)
	g++ $(STD) -o $@ $(MAIN).o -L. lib$(LIB)

$(MAIN).o : $(MAIN).cpp
	g++ $(STD) -o $@ $(MAIN).cpp -c

$(LIB) : chat$(PFX) message$(PFX) user$(PFX) sha1.o connection_config.o
	ar rc lib$(LIB) chat$(PFX) message$(PFX) user$(PFX) sha1.o connection_config.o

chat$(PFX) : chat$(PFX:.o=.cpp)
	g++ $(STD) -o $@ chat$(PFX:.o=.cpp) -c

message$(PFX) : message$(PFX:.o=.cpp)
	g++ $(STD) -o $@ message$(PFX:.o=.cpp) -c

user$(PFX) : user$(PFX:.o=.cpp)
	g++ $(STD) -o $@ user$(PFX:.o=.cpp) -c

sha1.o : sha1.cpp
	g++ $(STD) -o $@ sha1.cpp -c

connection_config.o : connection_config.cpp
	g++ $(STD) -o $@ connection_config.cpp -c

clean:	
	rm -rf *.o *.a 

exec:
	./server
