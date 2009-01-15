#include <iostream>
#include <fstream>
#include "time.h"
#include "graph_types.h"
#include "intersection.h"
#include "intersectiontest.h"

bool IntersectionTest::run(Graph &g) {
	Graph g1, g2;

	// g1
	Node *releaseNode = new Node("Release:semistable");
	releaseNode->setType(Entity::RELEASE);
	releaseNode = g1.addNode(releaseNode, Graph::FAIL_DUP);
	Node *componentNameNode = new Node("ComponentName:main");
	componentNameNode->setType(Entity::COMPONENTNAME);
	componentNameNode = g1.addNode(componentNameNode, Graph::FAIL_DUP);
	Node *componentNode = new Node("Component:semistable:main");
	componentNode->setType(Entity::COMPONENTNAME);
	componentNode = g1.addNode(componentNode, Graph::FAIL_DUP);
	// g1 edges
	g1.createEdge(releaseNode, componentNode, 
		Entity::CONTAINS, Edge::IGNORE_DUP);
	g1.createEdge(componentNameNode, componentNode,
		Entity::HAS_INSTANCE, Edge::IGNORE_DUP);
	// g2
	Node *releaseNode_g2 = new Node("Release:semistable");
	releaseNode_g2->setType(Entity::RELEASE);
	releaseNode_g2 = g2.addNode(releaseNode_g2, Graph::FAIL_DUP);

	Intersection isect = Intersection(g1, g2);
	Graph &result = isect.execute();

	// Do a timing test on the full graph
	/*
	time_t start,end;
	double diff = 0;
	char strdiff[10];
	time(&start);
	Intersection *isect_full = new Intersection(g, g);
	Graph &result_full = isect_full->execute();
	time(&end);
	diff = difftime(end, start);
	sprintf(strdiff, "%.0f", diff);
	cout << endl << "\tCompleted in " << strdiff << " seconds"
		 << endl;

	if (result.findNode("Release:semistable") == NULL) {
		return false;
	}
	if (g.size() != result_full.size()) {
		return false;
	}
	*/
	if (result.size() != 1) {
		return false;
	}
	return true;
}

