#include "union.h"

// NB: Constructor defined in header file.

Union::~Union() {
}

Graph& Union::execute() {
	// Begin with a copy of the result graph
	Graph *smaller;
	Graph *result;
	if (l_operand.size() > r_operand.size()) {
		result = new Graph(l_operand);
		smaller = &r_operand;
	}
	else {
		result = new Graph(r_operand);
		smaller = &l_operand;
	}
	registerObject(result);
	for (GraphIterator i = smaller->begin(); i != smaller->end(); ++i) {
		result->addNode(*i, Graph::DISCARD_DUP);
	}
	copyConsistentEdges(l_operand, r_operand, *result);
	return *result;
}
