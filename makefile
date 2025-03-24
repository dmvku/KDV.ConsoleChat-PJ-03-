STD = -std=c++17
PFX = _class.o
MAIN = chat_main
LIB = ChatLIB.a

chat : $(MAIN).o $(LIB)
	g++ $(STD) -o $@ $(MAIN).o -L. lib$(LIB)

$(MAIN).o : $(MAIN).cpp
	g++ $(STD) -o $@ $(MAIN).cpp -c

$(LIB) : chat$(PFX) message$(PFX) user$(PFX) exception$(PFX)
	ar rc lib$(LIB) chat$(PFX) message$(PFX) user$(PFX) exception$(PFX)

chat$(PFX) : chat$(PFX:.o=.cpp)
	g++ $(STD) -o $@ chat$(PFX:.o=.cpp) -c

message$(PFX) : message$(PFX:.o=.cpp)
	g++ $(STD) -o $@ message$(PFX:.o=.cpp) -c

user$(PFX) : user$(PFX:.o=.cpp)
	g++ $(STD) -o $@ user$(PFX:.o=.cpp) -c

exception$(PFX) : exception$(PFX:.o=.cpp)
	g++ $(STD) -o $@ exception$(PFX:.o=.cpp) -c

clean:	
	rm -rf *.o *.a 

exec:
	./chat
