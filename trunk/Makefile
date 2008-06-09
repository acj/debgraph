CXXFLAGS?=-g -Wall
LIBS=
LDFLAGS=
MAKE=make
CXX=g++
TARGET=debgraph
DEPMAKER=$(shell which fastdep || echo "$(CXX) -MM")

SOURCES=$(shell ls *.cc)
OBJECTS=$(subst .cc,.o,$(SOURCES))

export OBJECTS

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
	@echo "Running cleanup scripts..."
	-for s in `find scripts -name "C*" | grep -v "\.svn" | sort`; do $$s; done

distclean: clean
	$(MAKE) -C tests clean

setup:
	@echo "Running setup scripts..."
	-for s in `find scripts -name "S*" | grep -v "\.svn" | sort`; do $$s; done

run: $(TARGET) setup
	@echo "Running debgraph..."
	@./debgraph
	@echo "Running final scripts..."
	-for s in `find scripts -name "F*" | grep -v "\.svn" | sort`; do $$s; done

test: $(TARGET) setup
	$(MAKE) -C tests run

-include .depend
