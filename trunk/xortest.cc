#include <iostream>
#include <fstream>
#include "graph_types.h"
#include "xor.h"
#include "xortest.h"

bool XORTest::run(Graph &g) {
	Graph *g1 = new Graph();
	Graph *g2 = new Graph();
	Edge *e;

	// g1
	Node *releaseNode = new Node("Release:semistable");
	releaseNode->setType(Entity::RELEASE);
	releaseNode = g1->addNode(releaseNode, Graph::DISCARD_DUP);
	Node *componentNameNode = new Node("ComponentName:main");
	componentNameNode->setType(Entity::COMPONENTNAME);
	componentNameNode = g1->addNode(componentNameNode, Graph::DISCARD_DUP);
	Node *componentNode = new Node("Component:semistable:main");
	componentNode->setType(Entity::COMPONENTNAME);
	componentNode = g1->addNode(componentNode, Graph::DISCARD_DUP);
	// g2
	Node *releaseNode_g2 = new Node("Release:semistable");
	releaseNode_g2->setType(Entity::RELEASE);
	releaseNode_g2 = g2->addNode(releaseNode_g2, Graph::DISCARD_DUP);
	// edges
	e = Edge::createEdge(releaseNode, componentNode, 
		Entity::CONTAINS, Edge::IGNORE_DUP);
	e = Edge::createEdge(componentNameNode, componentNode,
		Entity::HAS_INSTANCE, Edge::IGNORE_DUP);
	XOR *x = new XOR(*g1, *g2);
	Graph *result = x->execute();	

	// Check sizes
	if (result->size() != 2) {
		cout << "\n\tIncorrect number of nodes in resulting graph: got " 
			 << result->size() << " but expected 2" << endl;
		return false;
	}
	// Check individual nodes
	if ( !result->hasNode("Release:semistable")
			&& result->hasNode("ComponentName:main")
			&& result->hasNode("Component:semistable:main") ) {
		return true;
	}
	else {
		cout << "\tIncorrect resulting Node set" << endl;
		return false;
	}
}
