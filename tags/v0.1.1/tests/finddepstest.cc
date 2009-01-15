#include <iostream>
#include <fstream>
#include "graph_types.h"
#include "finddeps.h"
#include "finddepstest.h"

bool FindDepsTest::run(Graph &g) {
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
	Graph result = FindDeps(depGraph, tree).execute();
	Node *n = result.findNode("Soil");
	if (n == NULL) {
		cerr << "Could not find node `Soil'" << endl;
		return false;
	}
	if (n->getId() != "Soil") {
		cerr << "Incorrect dependency node found" << endl;
		return false;
	}
	ofstream dotfile("out/finddeps.dot");
	dotfile << result.toGraphviz();
	dotfile.close();
	return true;
}
