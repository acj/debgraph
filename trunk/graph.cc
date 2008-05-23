#include "graph.h"

Graph::Graph() {
}

Graph::~Graph() {
	Node *n;
	GraphIndexT::iterator i;
	while (index.size() > 0) {
		i = index.begin();
		n = (*i).second;
		index.erase(index.begin());
		delete(n);
	}
}
		
Node* Graph::addNode(Node *node, AddFlag flag) {
	GraphIndexT::iterator i = index.find(node->getId());
	if (i != index.end()) {
		if (flag == FAIL_DUP) {
			throw "duplicate node";
		}
		else if (flag == DISCARD_DUP) {
			delete node;
			return i->second;
		}
		/* ignore_dup means we insert anyway */
	}
	index.insert(pair<string, Node*>(node->getId(), node));
	return node;
}

Node* Graph::findNode(const string &id) {
	GraphIndexT::iterator i = index.find(id);
	if (i != index.end()) {
		return i->second;
	}
	return NULL;
}

bool Graph::hasNode(const string &id) {
	GraphIndexT::iterator i = index.find(id);
	return (i != index.end());
}

int Graph::size() {
	return index.size();
}

GraphIndexT& Graph::getIndex() {
	return index;
}

/* Iterator-related members */
GraphIterator &Graph::begin() {
	GraphIndexIteratorT index_iter = index.begin();
	GraphIterator *iter = new GraphIterator();
	iter->setIndexIterator(index_iter);
	return *iter;
}

GraphIterator &Graph::end() {
	GraphIndexIteratorT index_iter = index.end();
	GraphIterator *iter = new GraphIterator();
	iter->setIndexIterator(index_iter);
	return *iter;
}
