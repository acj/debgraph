#include "filter.h"

Filter::Filter(Graph &oper1, FilterProperties filterProps, FilterType)
	: l_operand(oper1) {
	properties = filterProps;
}

Filter::~Filter() {
}

Graph& Filter::execute() {
	FilterPropertiesIterator iter;
	bool node_matches = true;
	for (GraphIterator graph_iter = l_operand.begin(); 
			graph_iter != l_operand.end(); ++graph_iter) {
		// Check each property
		if (filterType == FILTER_AND) {
			node_matches = true;	
			for (iter = properties.begin(); iter != properties.end(); ++iter) {
				if ((*graph_iter)->getProperty(iter->key) == iter->value) {
					continue;
				}
				else {
					node_matches = false;
					break;
				}
			}
		}
		else {
			// FILTER_OR
			node_matches = false;
			for (iter = properties.begin(); iter != properties.end(); ++iter) {
				if ((*graph_iter)->getProperty(iter->key) == iter->value) {
					// Match!
					node_matches = true;
					break;
				}
			}
		}
		if (node_matches) {
			result.addNode(new Node(**graph_iter), Graph::FAIL_DUP);
		}
	}
	return result;
}
