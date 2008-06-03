#include "intersection.h"
#include <iostream>

Intersection::Intersection(Graph &l_operand, Graph &r_operand) 
	: l_operand(l_operand), r_operand(r_operand)  {
}

Intersection::~Intersection() {
}

Graph& Intersection::execute() {
	Graph *smaller_g, *larger_g;

	if (l_operand.size() > r_operand.size()) {
		smaller_g = &r_operand;
		larger_g = &l_operand;
	}
	else { 
		// includes ==
		smaller_g = &l_operand;
		larger_g = &r_operand;
	}

	string node_id;
	for (GraphIterator i = smaller_g->begin(); i != smaller_g->end(); ++i) {
		node_id = (*i)->getId();
		if (larger_g->hasNode(node_id)) {
			result.addNode(new Node(**i), Graph::FAIL_DUP);
		}
	}
	return result;
}
