#include <iostream>
#include "graph.h"

Graph::Graph() {
}

Graph::Graph(Graph &g) {
	// First, copy the nodes
	for (GraphIndexIteratorT iter = g.index.begin(); iter != g.index.end(); ++iter) {
		index.insert(pair<string,Node*>(iter->first, new Node(*(iter->second))));
	}
	// Fix up the (outbound) edges
	string oldNearNodeId;
	Node *oldNearNode, *oldFarNode, *newNearNode, *newFarNode;
	Edge *newEdge;
	pair<string,Node*> old_node_entry;
	for (GraphIndexIteratorT iter = g.index.begin(); 
			iter != g.index.end(); 
			++iter) {
		old_node_entry = *iter;
		oldNearNodeId = old_node_entry.first;
		oldNearNode = old_node_entry.second;
		newNearNode = index.find(oldNearNodeId)->second;
		for (set<Edge*>::iterator iter = oldNearNode->getOutEdges().begin();
				iter != oldNearNode->getOutEdges().end();
				++iter) {
			oldFarNode = (Node *)(*iter)->getToNode();
			newFarNode = index.find(oldFarNode->getId())->second;
			newEdge = Edge::createEdge(newNearNode, newFarNode, (*iter)->getType(), Edge::IGNORE_DUP);
		}
	}
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

Graph& Graph::operator=(Graph &g) {
	cout << endl << "!!! Graph operator=() called: " << g.size() << endl;
	for (GraphIndexIteratorT iter = g.index.begin(); iter != g.index.end(); ++iter) {
		index.insert(pair<string,Node*>(iter->first, new Node(*(iter->second))));
	}
	return *this;
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

string Graph::toGraphviz() {
	string output("digraph \"DebGraph Output\" {\n");
	output += "nodesep=0.1\n";
	for (GraphIterator iter = this->begin(); iter != this->end(); ++iter) {
		output += (*iter)->toGraphviz();
	}
	output += "}";
	return output;
}
