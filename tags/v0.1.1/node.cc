#include <iostream>

#include "node.h"

Node::Node() {
	id = "";
	parentGraph = 0;
}

Node::Node(Node &node) {
	id = node.id;
	type = node.type;
	properties = node.properties;
	parentGraph = node.parentGraph;
}

Node::Node(const string &nodeId) {
	id = nodeId;
	parentGraph = 0;
}

Node::~Node() {
}

Node& Node::operator=(Node &n) {
	if (this != &n) {
		id = n.id;
		properties = n.properties;
		parentGraph = n.parentGraph;
	}
	return *this;
}

void Node::setId(string &nid) {
	id = nid;
}

const string& Node::getId() const {
	return id;
}	
