#include <iostream>
#include <fstream>
#include "graph_types.h"
#include "findcycles.h"
#include "findcyclestest.h"
#include "finddeps.h"

bool FindCyclesTest::run(Graph &g) {
	Graph g1;
	// Set up nodes (chicken cycle)
	Node *chicken, *egg, *nutrients, *soil;
 	chicken = new Node("Chicken");
	chicken->setType(Entity::BINARYNAME);
	chicken->addProperty("Package", "Chicken");
	g1.addNode(chicken, Graph::FAIL_DUP);
	egg = new Node("Egg");
	egg->setType(Entity::BINARYNAME);
	egg->addProperty("Package", "Egg");
	g1.addNode(egg, Graph::FAIL_DUP);
	nutrients = new Node("Nutrients");
	nutrients->setType(Entity::BINARYNAME);
	nutrients->addProperty("Package", "Nutrients");
	g1.addNode(nutrients, Graph::FAIL_DUP);
	soil = new Node("Soil");
	soil->setType(Entity::BINARYNAME);
	soil->addProperty("Package", "Soil");
	g1.addNode(soil, Graph::FAIL_DUP);
	// Set up nodes (plant cycle)
	Node *soil2, *plant, *bee, *pollen;
	soil2 = new Node("Soil2");
	soil2->setType(Entity::BINARYNAME);
	soil2->addProperty("Package", "Soil2");
	g1.addNode(soil2, Graph::FAIL_DUP);
	plant = new Node("Plant");
	plant->setType(Entity::BINARYNAME);
	plant->addProperty("Package", "Plant");
	g1.addNode(plant, Graph::FAIL_DUP);
	bee = new Node("Bee");
	bee->setType(Entity::BINARYNAME);
	bee->addProperty("Package", "Bee");
	g1.addNode(bee, Graph::FAIL_DUP);
	pollen = new Node("Pollen");
	pollen->setType(Entity::BINARYNAME);
	pollen->addProperty("Package", "Pollen");
	g1.addNode(pollen, Graph::FAIL_DUP);

	// Set up edges
	Edge::createEdge(chicken, egg, Entity::DEPENDS, Edge::IGNORE_DUP);
	Edge::createEdge(egg, chicken, Entity::DEPENDS, Edge::IGNORE_DUP);
	Edge::createEdge(chicken, nutrients, Entity::DEPENDS, Edge::IGNORE_DUP);
	Edge::createEdge(egg, nutrients, Entity::DEPENDS, Edge::IGNORE_DUP);
	Edge::createEdge(nutrients, soil, Entity::DEPENDS, Edge::IGNORE_DUP);
	Edge::createEdge(soil, chicken, Entity::DEPENDS, Edge::IGNORE_DUP);
	Edge::createEdge(pollen, plant, Entity::DEPENDS, Edge::IGNORE_DUP);
	Edge::createEdge(plant, bee, Entity::DEPENDS, Edge::IGNORE_DUP);
	Edge::createEdge(bee, pollen, Entity::DEPENDS, Edge::IGNORE_DUP);
	Edge::createEdge(plant, soil2, Entity::DEPENDS, Edge::IGNORE_DUP);
	Edge::createEdge(soil2, plant, Entity::DEPENDS, Edge::IGNORE_DUP);

	ofstream lifecycles("out/findcycles-full.dot");
	lifecycles << g1.toGraphviz();
	lifecycles.close();

	FindCycles fc(g1, FindCycles::DEPENDS, "Soil");
	Graph result = fc.execute();
	size_t cycleCount = fc.getCycles().size();
	cout << "\n\tFound " << cycleCount << " cycles" << endl;
	char cycleName[25];
	for (size_t i = 0; i<cycleCount; ++i) {
		sprintf(cycleName, "out/findcycles-%d.dot", i);
		ofstream dotfile(cycleName);
		dotfile << fc.getCycles()[i].toGraphviz();
		dotfile.close();
	}
	if (fc.getCycles().size() == 2) {
		return true;
	}
	else {
		cout << "\tIncorrect number of cycles found." << endl;
		return false;
	}
}
