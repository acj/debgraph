#include <iostream>
#include <fstream>
#include "graph_types.h"
#include "findreversedeps.h"
#include "findreversedepstest.h"

bool FindReverseDepsTest::run(Graph &g) {
	Graph depGraph;
	Node *tree = new Node("Tree");
	tree->setType(Entity::BINARY);
	tree->addProperty("Package", "Tree");
	Node *soil = new Node("Soil");
	soil->setType(Entity::BINARY);
	soil->addProperty("Package", "Soil");

	depGraph.addNode(tree);
	depGraph.addNode(soil);
	depGraph.createEdge(tree, soil, Entity::DEPENDS, Edge::IGNORE_DUP);
	Graph result = FindReverseDeps(depGraph, soil).execute();
	Node *n = result.findNode("Tree");
	if (n == NULL) {
		cerr << "Could not find node 'Tree'" << endl;
		return false;
	}
	if (n->getId() != "Tree") {
		cerr << "Incorrect dependent node found" << endl;
		return false;
	}
	ofstream dotfile("out/findreversedeps.dot");
	dotfile << result.toGraphviz();
	dotfile.close();
	return true;
}
