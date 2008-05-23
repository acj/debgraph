#include "union.h"

Union::Union(Graph &oper1, Graph &oper2) 
	: l_operand(oper1), r_operand(oper2)  {
}

Union::~Union() {
}

Graph* Union::execute() {
	Graph* result_graph = new Graph();
	for (GraphIterator i = l_operand.begin(); i != l_operand.end(); ++i) {
		result_graph->addNode(*i, Graph::DISCARD_DUP);
	}
	for (GraphIterator i = r_operand.begin(); i != r_operand.end(); ++i) {
		result_graph->addNode(*i, Graph::DISCARD_DUP);
	}
	return result_graph;
}
