#include "union.h"

Union::Union(Graph &oper1, Graph &oper2) 
	: l_operand(oper1), r_operand(oper2)  {
}

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
		result.addNode(new Node(**i), Graph::DISCARD_DUP);
	}
	return result;
}
