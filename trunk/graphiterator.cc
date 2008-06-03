#include "graphiterator.h"

GraphIterator::GraphIterator() {
}

GraphIterator& GraphIterator::operator++() {
	graph_iter++;		
	return *this;
}

bool GraphIterator::operator==(const GraphIterator &iter) const {
	if (graph_iter == iter.graph_iter) {
		return true;
	} else {
		return false;
	}
}

bool GraphIterator::operator!=(const GraphIterator &iter) const {
	if (graph_iter != iter.graph_iter) {
		return true;
	} else {
		return false;
	}
}

Node* GraphIterator::operator*() const {
	return (*graph_iter).second;
}
