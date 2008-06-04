#include <fstream>
#include <iostream>
#include "graph.h"
#include "graph_types.h"
#include "filter.h"
#include "filtertest.h"

bool FilterTest::run(Graph &g) {
	// Intentionally create an edge between two separate graphs.
	// Creating such an edge should fail with a useful error message.
	Graph g1, g2;
	Edge *e;
	// g1
	Node *releaseNode = new Node("Release:semistable");
	releaseNode->addProperty("id", "Release:semistable");
	releaseNode->setType(Entity::RELEASE);
	releaseNode = g1.addNode(releaseNode, Graph::DISCARD_DUP);
	Node *componentNameNode = new Node("ComponentName:main");
	componentNameNode->addProperty("id", "ComponentName:main");
	componentNameNode->setType(Entity::COMPONENTNAME);
	componentNameNode = g1.addNode(componentNameNode, Graph::DISCARD_DUP);
	Node *componentNode = new Node("Component:semistable:main");
	componentNode->addProperty("id", "Component:semistable:main");
	componentNode->setType(Entity::COMPONENTNAME);
	componentNode = g1.addNode(componentNode, Graph::DISCARD_DUP);
	e = Edge::createEdge(releaseNode, componentNode, 
		Entity::CONTAINS, Edge::IGNORE_DUP);
	e = Edge::createEdge(componentNameNode, componentNode,
		Entity::HAS_INSTANCE, Edge::IGNORE_DUP);

	FilterProperties fp;
	FilterPair fpair = { string("id"), string("Component:semistable:main") };
	fp.push_back(fpair);
	Filter f(g1, fp, FILTER_AND);
	Graph result = f.execute();

	if (result.size() == 1
		&& result.hasNode("Component:semistable:main")
		&& !result.hasNode("ComponentName:main")
		&& !result.hasNode("Release:semistable")) {
		return true;
	}
	else {
		return false;
	}
}
