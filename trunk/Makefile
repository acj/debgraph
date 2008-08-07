CXXFLAGS?=-g -Wall
LIBS=
LDFLAGS=
MAKE=make
CXX=g++
TARGET=debgraph_test
DEPMAKER=$(shell which fastdep || echo "$(CXX) -MM")
INSTALL=/usr/bin/install

bindir=/usr/bin
libdir=/usr/lib

SOURCES=$(shell ls *.cc)
OBJECTS=$(subst .cc,.o,$(SOURCES))

export OBJECTS

all: $(TARGET) lua

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
	$(MAKE) -C doc clean
	$(MAKE) -C lua clean
	$(MAKE) -C tests clean

distclean: clean
	@echo "Running cleanup scripts..."
	-for s in `find scripts -name "C*" | grep -v "\.svn" | sort`; do $$s; done

install:
	$(INSTALL) -m 755 -d /usr/share/debgraph/cache
	$(INSTALL) -m 755 -d /usr/share/debgraph/scripts
	$(INSTALL) -s -m 755 lua/debgraph_cli $(bindir)/debgraph_cli
	$(INSTALL) -m 755 debgraph $(bindir)/debgraph
	$(INSTALL) -m 755 lua/libdebgraph.so $(libdir)/libdebgraph.so.0
	@ln -f -s $(libdir)/libdebgraph.so.0 $(libdir)/libdebgraph.so
	$(MAKE) -C doc html
	$(INSTALL) -D -m 755 doc/html/* -t /usr/share/doc/debgraph
	$(INSTALL) -m 755 -t /usr/share/debgraph/scripts scripts/{C*,S*,F*}

lua: $(TARGET)
	$(MAKE) -C lua
	
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
