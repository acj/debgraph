TOPDIR=../
INCLUDES=`lua-config --include` -I../
CXXFLAGS?=-g -Wall
LDFLAGS=`lua-config --libs`
MAKE=make
CXX=g++
# Remove the top-level main() object and prepend a directory prefix on 
# upstream objects.
DG_OBJECTS=$(patsubst %.o,$(TOPDIR)%.o,$(subst main.o,,$(OBJECTS)))

debgraph_cli: debgraph_cli.o libdebgraph.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(INCLUDES) $^ $(DG_OBJECTS) -o $@

%.o: %.c
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $<

clean:
	rm -f *.o
	rm -f $(TARGET)
