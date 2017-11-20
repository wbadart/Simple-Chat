CXX_FLAGS = -Wall -ggdb -g -O0 -std=gnu++11 -I../../include -Wno-write-strings
LD_FLAGS  = -lpthread


all: server

server: server.o ../userdb.o ../utils.o
	g++ $(LD_FLAGS) -o $@ $^

%.o: %.cpp
	g++ $(CXX_FLAGS) -c -o $@ $^

clean:
	rm -rf *.o server

