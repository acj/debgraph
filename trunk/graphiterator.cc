#include "graphiterator.h"

GraphIterator::GraphIterator() {
}

//GraphIterator& GraphIterator::operator=(Graph* graph_oper) {
	// XXX
//}

GraphIterator& GraphIterator::operator++() {
	graph_iter++;		
	return *this;
}

bool GraphIterator::operator==(const GraphIterator &iter) {
	if (graph_iter == iter.graph_iter) {
		return true;
	} else {
		return false;
	}
}

bool GraphIterator::operator!=(const GraphIterator &iter) {
	if (graph_iter != iter.graph_iter) {
		return true;
	} else {
		return false;
	}
}

Node* GraphIterator::operator*() {
	return (*graph_iter).second;
}
