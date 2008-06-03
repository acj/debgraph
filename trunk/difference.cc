#include "difference.h"

Difference::Difference(Graph &oper1, Graph &oper2)
	: l_operand(oper1), r_operand(oper2) {
}

Difference::~Difference() {
}

Graph& Difference::execute() {
	result = l_operand;
	string searchNodeId;
	for (GraphIterator i = r_operand.begin(); i != r_operand.end(); ++i) {
		searchNodeId = (*i)->getId();
		if (result.hasNode(searchNodeId)) {
			result.getIndex().erase(searchNodeId);
		}
	}
	return result;
}
