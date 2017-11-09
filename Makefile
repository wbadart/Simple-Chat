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

CXX		  = g++
CXX_FLAGS = -Wall -ggdb -O0 -std=gnu++11 -I./include
LD		  = g++
LD_FLAGS  = -L./lib -luserdatabase
AR		  = ar
AR_FLAGS  = rvs

all: $(LIBS) $(PROGS)


bin/%: src/%.o
	@test -d bin || mkdir bin
	$(LD) $< -o $@ $(LD_FLAGS)

lib/%.a: src/%.o
	@test -d lib || mkdir lib
	$(AR) $(AR_FLAGS) $@ $^

src/%.o: src/%.cpp
	$(CXX) $(CXX_FLAGS) -c -o $@ $^

clean:
	rm -rf lib/ bin/ src/*.o
