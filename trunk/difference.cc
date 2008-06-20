#include "difference.h"

// NB: Constructor defined in header file.

Difference::~Difference() {
}

Graph& Difference::execute() {
	result = l_operand;
	string searchNodeId;
	for (GraphIterator i = r_operand.begin(); i != r_operand.end(); ++i) {
		searchNodeId = (*i)->getId();
		if (result.hasNode(searchNodeId)) {
			result.deleteNode(searchNodeId);
		}
	}
	copyConsistentEdges(l_operand, r_operand, result);
	return result;
}
