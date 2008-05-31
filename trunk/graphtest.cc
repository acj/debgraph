#include <fstream>
#include <iostream>
#include "graph_types.h"
#include "graph.h"
#include "graphtest.h"

bool GraphTest::run(Graph &g) {
	Graph *g1 = new Graph();
	Edge *e;

	Node *releaseNode = new Node("Release:semistable");
	releaseNode->setType(Entity::RELEASE);
	releaseNode = g1->addNode(releaseNode, Graph::DISCARD_DUP);
	Node *componentNameNode = new Node("ComponentName:main");
	componentNameNode->setType(Entity::COMPONENTNAME);
	componentNameNode = g1->addNode(componentNameNode, Graph::DISCARD_DUP);
	Node *componentNode = new Node("Component:semistable:main");
	componentNode->setType(Entity::COMPONENTNAME);
	componentNode = g1->addNode(componentNode, Graph::DISCARD_DUP);
	e = Edge::createEdge(releaseNode, componentNode, 
		Entity::CONTAINS, Edge::IGNORE_DUP);
	e = Edge::createEdge(componentNameNode, componentNode,
		Entity::HAS_INSTANCE, Edge::IGNORE_DUP);
	
	ofstream dotfile;
	dotfile.open("out/graphtest-orig.dot");
	dotfile << g1->toGraphviz();
	dotfile.close();

	// XXX Make sure that edges cannot exist between different graphs
	
	// Test the copy constructor
	Graph *g1_copy = new Graph(*g1);
	delete g1;
	if (g1_copy->findNode("Release:semistable") == NULL
		|| g1_copy->findNode("ComponentName:main") == NULL
		|| g1_copy->findNode("Component:semistable:main") == NULL
		|| g1_copy->size() != 3) {
		cout << "\n\tFailed to copy Graph object." << endl;
		return false;
	}

	dotfile.open("out/graphtest-copy.dot");
	dotfile << g1_copy->toGraphviz();
	dotfile.close();
	
	delete g1_copy;
	return true;
}
