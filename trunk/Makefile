CXXFLAGS?=-g -Wall
LIBS=
LDFLAGS=
CXX=g++
TARGET=debgraph
DEPMAKER=$(shell which fastdep || echo "$(CXX) -MM")

SOURCES=$(shell ls *.cc)
OBJECTS=$(subst .cc,.o,$(SOURCES))

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LIBS) -o $@ $^

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $<

.depend: *.cc *.h
	$(DEPMAKER) *.cc > .depend

.PHONY: clean run
clean:
	rm -f *.o
	rm -f $(TARGET)
	rm -f .depend

run: $(TARGET)
	./run

-include .depend

