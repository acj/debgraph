#include <fstream>
#include <iostream>
#include "graph_types.h"
#include "graph.h"
#include "nodetest.h"

bool NodeTest::run(Graph &g) {
	Node *n = new Node("foo");
	if ( !(n->getId() == "foo") ) {
		cout << "\tID or parent graph failed to initialize" << endl;
		return false;
	}
	n->setType(Entity::BINARYNAME);
	if ( !(n->getType() == Entity::BINARYNAME
			&& n->getTypeString() == "BINARYNAME") ) {
		cout << "\tsetType()/getType()/getTypeString() failed" << endl;
		return false;
	}

	Node *nCopy = new Node(*n);
	if ( !(nCopy->getType() == Entity::BINARYNAME
			&& nCopy->getTypeString() == "BINARYNAME") ) {
		cout << "\tCopy constructor failed" << endl;
		return false;
	}
	return true;
}
