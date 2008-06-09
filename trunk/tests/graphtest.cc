#include <fstream>
#include <iostream>
#include "graph_types.h"
#include "graph.h"
#include "graphtest.h"

bool GraphTest::run(Graph &g) {
	Graph addDeleteGraph;
	Node *releaseNode = new Node("Release:semistable");
	releaseNode->setType(Entity::RELEASE);
	releaseNode = addDeleteGraph.addNode(releaseNode, Graph::DISCARD_DUP);

	if ( !(addDeleteGraph.size() == 1
		&& addDeleteGraph.hasNode("Release:semistable")) ) {
		cout << "Add/delete Node failed" << endl;
		return false;
	}
	addDeleteGraph.deleteNode("Release:semistable");
	if ( !(addDeleteGraph.size() == 0
		&& !addDeleteGraph.hasNode("Release:semistable")) ) {
		return false;
	}

	// Test the copy constructor
	Graph g1;
	Edge *e;

	releaseNode = new Node("Release:semistable");
	releaseNode->setType(Entity::RELEASE);
	releaseNode = g1.addNode(releaseNode, Graph::DISCARD_DUP);
	Node *componentNameNode = new Node("ComponentName:main");
	componentNameNode->setType(Entity::COMPONENTNAME);
	componentNameNode = g1.addNode(componentNameNode, Graph::DISCARD_DUP);
	Node *componentNode = new Node("Component:semistable:main");
	componentNode->setType(Entity::COMPONENTNAME);
	componentNode = g1.addNode(componentNode, Graph::DISCARD_DUP);
	e = Edge::createEdge(releaseNode, componentNode, 
		Entity::CONTAINS, Edge::IGNORE_DUP);
	e = Edge::createEdge(componentNameNode, componentNode,
		Entity::HAS_INSTANCE, Edge::IGNORE_DUP);
	ofstream dotfile;
	dotfile.open("out/graphtest-orig.dot");
	dotfile << g1.toGraphviz();
	dotfile.close();

	Graph g1_copy(g1);
	if ( !(g1_copy.hasNode("Release:semistable")
		&& g1_copy.findNode("Release:semistable") != NULL
		&& g1_copy.hasNode("ComponentName:main")
		&& g1_copy.findNode("ComponentName:main") != NULL
		&& g1_copy.hasNode("Component:semistable:main")
		&& g1_copy.findNode("Component:semistable:main") != NULL
		&& g1_copy.size() == 3) ) {
		cout << "\n\tFailed to copy Graph object." << endl;
		return false;
	}
	// Verify that the copied nodes are not pointers to an old node
	Node *n;
	for (GraphIterator iter = g1.begin(); iter != g1.end(); ++iter) {
		n = g1_copy.findNode((*iter)->getId());
		if (*iter == n) {
			cout << "New Graph contains pointer to Node in old Graph" << endl;
			return false;
		}
	}
	dotfile.open("out/graphtest-copy.dot");
	dotfile << g1_copy.toGraphviz();
	dotfile.close();
	return true;
}
