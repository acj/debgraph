#include <fstream>
#include <iostream>
#include "graph.h"
#include "graph_types.h"
#include "difference.h"
#include "differencetest.h"

bool DifferenceTest::run(Graph &g) {
	// Intentionally create an edge between two separate graphs.
	// Creating such an edge should fail with a useful error message.
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
	e = Edge::createEdge(releaseNode, componentNode, 
		Entity::CONTAINS, Edge::IGNORE_DUP);
	e = Edge::createEdge(componentNameNode, componentNode,
		Entity::HAS_INSTANCE, Edge::IGNORE_DUP);
	// g2
	Node *releaseNode_g2 = new Node("Release:semistable");
	releaseNode_g2->setType(Entity::RELEASE);
	releaseNode_g2 = g2->addNode(releaseNode_g2, Graph::DISCARD_DUP);

	Difference *d = new Difference(*g1, *g2);
	Graph *result = d->execute();

	if (result->size() == 2
		&& result->hasNode("ComponentName:main")
		&& result->hasNode("Component:semistable:main")
		&& !result->hasNode("Release:semistable")) {
		return true;
	}
	else {
		return false;
	}
}
