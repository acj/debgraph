#include "union.h"

Union::Union(Graph &oper1, Graph &oper2) 
	: l_operand(oper1), r_operand(oper2)  {
}

Union::~Union() {
}

Graph* Union::execute() {
	// Begin with a copy of the result graph
	Graph *result, *smaller;
	if (l_operand.size() > r_operand.size()) {
		result = new Graph(l_operand);
		smaller = new Graph(r_operand);
	}
	else {
		result = new Graph(r_operand);
		smaller = new Graph(l_operand);
	}
	for (GraphIterator i = smaller->begin(); i != smaller->end(); ++i) {
		result->addNode(*i, Graph::DISCARD_DUP);
	}
	return result;
}
