#include "intersection.h"
#include <iostream>

// NB: Constructor defined in header file.

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
			result.addNode(*i, Graph::FAIL_DUP);
		}
	}
	copyConsistentEdges(l_operand, r_operand, result);
	return result;
}
