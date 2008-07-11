TOPDIR=../
# For Lua 5.0:
#INCLUDES=`lua-config --include` -I../
#LDFLAGS=`lua-config --libs`
# For Lua 5.1:
INCLUDES=-I/usr/include/lua5.1 -I../
LDFLAGS=-llua5.1 #`lua-config --libs`
CXXFLAGS?=-g -Wall
MAKE=make
CXX=g++
# Remove the top-level main() object and prepend a directory prefix on 
# upstream objects.
DG_OBJECTS=$(patsubst %.o,$(TOPDIR)%.o,$(subst main.o,,$(OBJECTS)))

all: libdebgraph.so debgraph_cli

debgraph_cli: debgraph_cli.o libdebgraph.o libdebgraph.so
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(INCLUDES) $^ $(DG_OBJECTS) -o $@

libdebgraph.so: libdebgraph.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(INCLUDES) $^ $(DG_OBJECTS) -shared -fPIC -o $@

%.o: %.cc
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $<

clean:
	rm -f *.o
	rm -f debgraph_cli
	rm -f libdebgraph.so
