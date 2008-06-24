#include <fstream>
#include <iostream>
#include "graph_types.h"
#include "graph.h"
#include "nodetest.h"

bool NodeTest::run(Graph &g) {
	Node *n = new Node("foo");
	if ( !(n->getId() == "foo"
			&& n->getParentGraph() == 0) ) {
		cout << "\tID or parent graph failed to initialize" << endl;
		return false;
	}
	n->setType(Entity::BINARYNAME);
	if ( !(n->getType() == Entity::BINARYNAME
			&& n->getTypeString() == "BINARYNAME") ) {
		cout << "\tsetType()/getType()/getTypeString() failed" << endl;
		return false;
	}
	n->setParentGraph(&g);
	if ( !(n->getParentGraph() == &g) ) {
		cout << "\tsetParentGraph()/getParentGraph() failed" << endl;
		return false;
	}

	Node *nCopy = new Node(*n);
	if ( !(nCopy->getType() == Entity::BINARYNAME
			&& nCopy->getTypeString() == "BINARYNAME"
			&& nCopy->getParentGraph() == &g) ) {
		cout << "\tCopy constructor failed" << endl;
		return false;
	}
	return true;
}
