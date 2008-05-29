#include <iostream>
#include <fstream>
#include "graph_types.h"
#include "union.h"
#include "uniontest.h"

bool UnionTest::run(Graph &g) {
	Graph *g1 = new Graph();
	Graph *g2 = new Graph();

	// XXX Add some edges for a more robust test
	Node *n = new Node("Release:foo");
	Node *n2 = new Node("Component:foo");


	n = g1->addNode(n, Graph::DISCARD_DUP);
	n2 = g2->addNode(n2, Graph::DISCARD_DUP);

	Union *u = new Union(*g1, *g2);
	Graph *result = u->execute();	

	ofstream ofile;
	ofile.open("out/uniontest.html");

	ofile << "<h1>uniontest</h2>" << endl;
	ofile << "First graph contains " << g1->size() << " nodes<br>" << endl;
	ofile << "Second graph contains " << g2->size() << " nodes<br>" << endl;
	ofile << "Combined graph contains " << result->size() << " nodes<br>" << endl;
	ofile << endl;
	ofile.close();

	for (GraphIterator i = result->begin(); i != result->end(); ++i) {
		Node *m = *i;
		cout << "Node: " << m->getId() << endl;
	}

	addItem("uniontest.html", "", "UnionTest");
	return true;
}

