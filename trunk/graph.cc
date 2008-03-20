#include "graph.h"

Graph::Graph() {
}

Graph::~Graph() {
	Node *n;
	multimap<string, Node*>::iterator i;
	while (index.size() > 0) {
		i = index.begin();
		n = (*i).second;
		index.erase(index.begin());
		delete(n);
	}
}
		
Node* Graph::addNode(Node *node, AddFlag flag) {
	multimap<string, Node*>::iterator i = index.find(node->getId());
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
	multimap<string, Node*>::iterator i = index.find(id);
	if (i != index.end()) {
		return i->second;
	}
	return NULL;
}

bool Graph::hasNode(const string &id) {
	multimap<string, Node*>::iterator i = index.find(id);
	return (i != index.end());
}

int Graph::size() {
	return index.size();
}

multimap<string, Node*>& Graph::getIndex() {
	return index;
}
