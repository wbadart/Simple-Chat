##
# Makefile
#
# Build rules for SimpleChat client, server, and library.
#
# Rob Simari
# Royce Branning
# Will Badart
#
# created: NOV 2017
##

PROGS = bin/server bin/client
LIBS  = lib/libuserdatabase.a

CXX       = g++
CXX_FLAGS = -Wall -ggdb -g -O0 -std=gnu++11 -I./include
LD        = g++
LD_FLAGS  = -L./lib -luserdatabase
AR        = ar
AR_FLAGS  = rvs

all: $(PROGS)


bin/client: src/client.o src/client_utils.o src/utils.o
	@test -d bin || mkdir bin
	$(LD) src/client.o src/client_utils.o src/utils.o -o bin/client

bin/server: src/server.o src/utils.o $(LIBS)
	@test -d bin || mkdir bin
	$(LD) src/server.o src/utils.o $(LD_FLAGS) -o bin/server

lib/%.a: src/%.o
	@test -d lib || mkdir lib
	$(AR) $(AR_FLAGS) $@ $^

src/%.o: src/%.cpp
	$(CXX) $(CXX_FLAGS) -c -o $@ $^

clean:
	rm -rf lib/ bin/ src/*.o
