INC_SVR_COMM = -I.  -I/usr/local/include
LIB_SVR_COMM = -L/usr/local/lib 

INC_ALL=$(INC_SVR_COMM) 
LIB_ALL=$(LIB_SVR_COMM)

BINARY = consistent_test

all:$(BINARY)


.SUFFIXES: .o .cpp
CXX = g++
CC = gcc

CXXFLAGS= -g -Wall -ggdb3 -DDEBUG_TEST -D_debug_on

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c $^ $(INC_ALL)

consistent_test:consistent_hash_test.o  hash.o
	$(CXX) $(CXXFLAGS)  -o $@ $^ $(LIB_ALL)
clean:
	rm -f *.o *~ $(BINARY)

strip:
	strip $(BINARY)
