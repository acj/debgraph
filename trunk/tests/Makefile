TOPDIR=../
INCLUDES=-I../
CXXFLAGS?=-g -Wall $(INCLUDES)
LDFLAGS=
MAKE=make
CXX=g++
TARGET=testsuite
DEPMAKER=$(shell which fastdep || echo "$(CXX) -MM")
TEST_SOURCES=$(shell ls *.cc)
TEST_OBJECTS=$(subst .cc,.o,$(TEST_SOURCES))
# Remove the top-level main() object and prepend a directory prefix on 
# upstream objects.
DG_OBJECTS=$(patsubst %.o,$(TOPDIR)%.o,$(subst main.o,,$(OBJECTS)))

$(TARGET): $(TEST_OBJECTS)
	$(CXX) $(INCLUDES) $(CXXFLAGS) $(LDFLAGS) $^ $(DG_OBJECTS) -o $(TARGET) 

%.o: %.cc
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $<

.depend: *.cc *.h
	$(DEPMAKER) $(INCLUDES) *.cc > .depend

clean:
	rm -f *.o
	rm -f $(TARGET)
	rm -f .depend

run: $(TARGET)
	@echo "Running test suite..."
	@./testsuite

-include .depend
