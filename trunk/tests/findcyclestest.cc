#include <iostream>
#include <fstream>
#include "graph_types.h"
#include "findcycles.h"
#include "findcyclestest.h"
#include "finddeps.h"

bool FindCyclesTest::run(Graph &g) {
	Graph g1;
	// Set up nodes
	Node *chicken, *egg, *nutrients, *soil;
 	chicken = new Node("Chicken");
	chicken->setType(Entity::RELEASE);
	chicken->addProperty("Package", "Chicken");
	g1.addNode(chicken, Graph::FAIL_DUP);
	egg = new Node("Egg");
	egg->setType(Entity::RELEASE);
	egg->addProperty("Package", "Egg");
	g1.addNode(egg, Graph::FAIL_DUP);
	nutrients = new Node("Nutrients");
	nutrients->setType(Entity::RELEASE);
	nutrients->addProperty("Package", "Nutrients");
	g1.addNode(nutrients, Graph::FAIL_DUP);
	soil = new Node("Soil");
	soil->setType(Entity::RELEASE);
	soil->addProperty("Package", "Soil");
	g1.addNode(soil, Graph::FAIL_DUP);

	// Set up edges
	Edge::createEdge(chicken, egg, Entity::DEPENDS, Edge::IGNORE_DUP);
	Edge::createEdge(egg, chicken, Entity::DEPENDS, Edge::IGNORE_DUP);
	Edge::createEdge(chicken, nutrients, Entity::DEPENDS, Edge::IGNORE_DUP);
	Edge::createEdge(egg, nutrients, Entity::DEPENDS, Edge::IGNORE_DUP);
	Edge::createEdge(nutrients, soil, Entity::DEPENDS, Edge::IGNORE_DUP);
	Edge::createEdge(soil, chicken, Entity::DEPENDS, Edge::IGNORE_DUP);

	ofstream chickencycle("out/findcycles-full.dot");
	chickencycle << g1.toGraphviz();
	chickencycle.close();

	FindCycles fc = FindCycles(g1);
	Graph result = fc.execute();
	cout << "\n\tFound " << fc.getCycles().size() << " cycles" << endl;
	if (fc.getCycles().size() == 3) {
		char cycleName[25];
		for (unsigned int i = 0; i<3; ++i) {
			sprintf(cycleName, "out/findcycles-%d.dot", i);
			ofstream dotfile(cycleName);
			dotfile << fc.getCycles()[i].toGraphviz();
			dotfile.close();
		}
		return true;
	}
	else {
		cout << "\tIncorrect number of cycles found." << endl;
		return false;
	}
}
