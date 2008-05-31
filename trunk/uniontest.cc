#include <iostream>
#include <fstream>
#include "graph_types.h"
#include "union.h"
#include "uniontest.h"

bool UnionTest::run(Graph &g) {
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
	Node *componentNode_g2 = new Node("Component:semistable:main");
	componentNode_g2->setType(Entity::COMPONENTNAME);
	componentNode_g2 = g2->addNode(componentNode_g2, Graph::DISCARD_DUP);
	// edges
	e = Edge::createEdge(releaseNode, componentNode, 
		Entity::CONTAINS, Edge::IGNORE_DUP);
	e = Edge::createEdge(componentNameNode, componentNode,
		Entity::HAS_INSTANCE, Edge::IGNORE_DUP);
	Union *u = new Union(*g1, *g2);
	Graph *result = u->execute();	

	// Check sizes
	if (result->size() != 3) {
		cout << "\n\tIncorrect number of nodes in resulting graph: got " 
			 << result->size() << " but expected 3" << endl;
		return false;
	}
	// Check individual nodes
	if ( !(result->findNode("Release:semistable") != NULL
			&& result->findNode("ComponentName:main") != NULL
			&& result->findNode("Component:semistable:main") != NULL) ) {
		cout << "\tMissing at least one expected node" << endl;
		return false;
	}
	return true;
}
