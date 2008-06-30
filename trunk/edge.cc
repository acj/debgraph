#include <iostream>
#include <stdint.h>
#include "edge.h"
#include "node.h"

Edge::Edge() {
	to = NULL;
	from = NULL;
}

Edge *Edge::createEdge(Node *fromNode, Node *toNode, Entity::EntityType type, CreateFlag flag) {
	if (fromNode->getParentGraph() != toNode->getParentGraph()) {
		cerr << "\n\tcreateEdge() failed: cannot create edges across graphs (" 
			 << fromNode->getParentGraph() << " vs. " << toNode->getParentGraph() << ")"
			 << endl;
		return NULL;
	}
	Edge *e = new Edge();
	if (flag == IGNORE_DUP) {
		set<Edge*> edges = fromNode->getOutEdges();
		set<Edge*>::iterator it = edges.begin();
		while (it != edges.end()) {
			if (((*it)->getType() == type) 
				&& ((*it)->getToNode() == toNode)) {
				delete e;
				return NULL;
			}
			it++;
		}
	}
	/* ok, add the edge */
	e->from = fromNode;
	e->to = toNode;
	e->setType(type);
	fromNode->getOutEdges().insert(fromNode->getOutEdges().begin(), e);
	toNode->getInEdges().insert(toNode->getInEdges().begin(), e);
	return e;
}

Edge::~Edge() {
	if (to != NULL) {
		to->getInEdges().erase(this);
	}
	if (from != NULL) {
		from->getOutEdges().erase(this);
	}
}

const Node *Edge::getFromNode() {
	return from;
}

const Node *Edge::getToNode() {
	return to;
}

string Edge::toGraphviz() {
	char source_gid[11], sink_gid[11];
	// Use pointer value as unique id
	sprintf(source_gid, "%u", (unsigned int)from);
	sprintf(sink_gid, "%u", (unsigned int)to);
	string output("\t node");
	output += source_gid + string(" -> node") + sink_gid;
	output += " [ label = \"" + this->getTypeString() + "\" ];\n";
	return output;
}
