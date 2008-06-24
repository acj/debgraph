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
	while (inEdges.size() > 0) {
		delete(*inEdges.begin());
	}
	while (outEdges.size() > 0) {
		delete(*outEdges.begin());
	}
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

Graph* Node::getParentGraph() const {
	return parentGraph;
}

void Node::setParentGraph(Graph* g) {
	parentGraph = g;
}
		
set<Edge*>& Node::getInEdges() {
	return inEdges;
}

set<Edge*>& Node::getOutEdges() {
	return outEdges;
}

bool Node::hasEdgeFrom(Node *n) const {
	for (set<Edge*>::const_iterator i = inEdges.begin(); 
			i != inEdges.end(); ++i) {
		if ((*i)->getFromNode() == n) {
			return true;
		}
	}
	return false;
}

bool Node::hasEdgeTo(Node *n) const {
	for (set<Edge*>::const_iterator i = outEdges.begin(); 
			i != outEdges.end(); ++i) {
		if ((*i)->getToNode() == n) {
			return true;
		}
	}
	return false;
}

string Node::toGraphviz() {
	string output("\tnode");
	char node_id_salt[10]; // XXX Make safe for 64-bit architectures
	sprintf(node_id_salt, "%u", (unsigned int)this);
	output += string(node_id_salt) + "[";
	if (getProperty("Package") == "" && getType() == Entity::OR) {
		output += "shape=circle, label=\"OR\" ";
	}
	else {
		output += "label = \"" + getProperty("Package") 
			+ "\\n" + getProperty("Version") 
			+ "\\n" + getProperty("Architecture");
	}
	output += "\"];\n";
	for (set<Edge*>::iterator i = outEdges.begin(); i != outEdges.end(); ++i) {
		output += (*i)->toGraphviz();	
	}
	return output;
}
