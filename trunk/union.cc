#include "union.h"

// NB: Constructor defined in header file.

Union::~Union() {
}

Graph& Union::execute() {
	// Begin with a copy of the result graph
	Graph *smaller;
	if (l_operand.size() > r_operand.size()) {
		result = l_operand;
		smaller = &r_operand;
	}
	else {
		result = r_operand;
		smaller = &l_operand;
	}
	for (GraphIterator i = smaller->begin(); i != smaller->end(); ++i) {
		result.addNode(*i, Graph::DISCARD_DUP);
	}
	copyConsistentEdges(l_operand, r_operand, result);
	return result;
}
