#include <iostream>

#include "node.h"

Node::Node() {
	id = "";
}

Node::Node(Node &node) {
	id = node.id;
	dfs = node.dfs;
	low = node.low;
	mark = node.mark;
}

Node::Node(const string &nodeId) {
	mark = 0;
	id = nodeId;
}

Node::~Node() {
	while (inEdges.size() > 0) {
		delete(*inEdges.begin());
	}
	while (outEdges.size() > 0) {
		delete(*outEdges.begin());
	}
}

Node& Node::operator=(Node &n) {
	id = n.id;
	dfs = n.dfs;
	low = n.low;
	mark = n.mark;
	return *this;
}

void Node::setId(string &nid) {
	id = nid;
}

const string& Node::getId() {
	return id;
}	
		
set<Edge*>& Node::getInEdges() {
	return inEdges;
}

set<Edge*>& Node::getOutEdges() {
	return outEdges;
}

void Node::setDfs(int d) {
	dfs = d;
}

int Node::getDfs() {
	return dfs;
}

void Node::setLow(int l) {
	low = l;
}

int Node::getLow() {
	return low;
}

void Node::setMark(int m) {
	mark = m;
}

int Node::getMark() {
	return mark;
}
		
