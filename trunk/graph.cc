#include <assert.h>
#include <iostream>
#include "graph.h"

Graph::Graph() {
}

Graph::Graph(const Graph &g) {
	clone(g);
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

Graph& Graph::operator=(const Graph &g) {
	if (this != &g) {
		clone(g);
	}
	return *this;
}

void Graph::clone(const Graph &g) {
	// First, copy the nodes
	for (GraphIterator iter = g.begin(); iter != g.end(); ++iter) {
		addNode(new Node(**iter), FAIL_DUP); 
	}
	// Fix up the (outbound) edges
	string oldNearNodeId;
	Node *oldNearNode, *oldFarNode, *newNearNode, *newFarNode;
	Edge *newEdge;
	for (GraphIterator iter = g.begin(); 
			iter != g.end(); 
			++iter) {
		oldNearNode = *iter;
		oldNearNodeId = oldNearNode->getId();
		newNearNode = findNode(oldNearNodeId);
		if (newNearNode == NULL) {
			throw("Matching new node is missing");
		}
		for (set<Edge*>::iterator iter = oldNearNode->getOutEdges().begin();
				iter != oldNearNode->getOutEdges().end();
				++iter) {
			oldFarNode = (Node *)(*iter)->getToNode();
			newFarNode = index.find(oldFarNode->getId())->second;
			assert(newNearNode != NULL && newFarNode != NULL);
			newEdge = Edge::createEdge(newNearNode, newFarNode, (*iter)->getType(), Edge::IGNORE_DUP);
		}
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
	node->setParentGraph(this);
	index.insert(pair<string, Node*>(node->getId(), node));
	return node;
}

Node* Graph::findNode(const string &id) const {
	GraphIndexConstIteratorT i = index.find(id);
	if (i != index.end()) {
		return i->second;
	}
	return NULL;
}

bool Graph::hasNode(const string &id) const {
	GraphIndexConstIteratorT i = index.find(id);
	return (i != index.end());
}

int Graph::size() const {
	return index.size();
}

GraphIndexT& Graph::getIndex() {
	return index;
}

/* Iterator-related members */
GraphIterator &Graph::begin() const {
	GraphIndexConstIteratorT index_iter = index.begin();
	GraphIterator *iter = new GraphIterator();
	iter->setIndexIterator(index_iter);
	return *iter;
}

GraphIterator &Graph::end() const {
	GraphIndexConstIteratorT index_iter = index.end();
	GraphIterator *iter = new GraphIterator();
	iter->setIndexIterator(index_iter);
	return *iter;
}

string Graph::toGraphviz() {
	string output("digraph \"DebGraph Output\" {\n");
	output += "nodesep=0.1\n";
	for (GraphIterator iter = this->begin(); iter != this->end(); ++iter) {
		output += (*iter)->toGraphviz();
	}
	output += "}";
	return output;
}
