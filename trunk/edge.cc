#include <iostream>

#include "edge.h"
#include "node.h"

Edge::Edge() {
	to = NULL;
	from = NULL;
}

Edge *Edge::createEdge(Node *fromNode, Node *toNode, Entity::EntityType type, CreateFlag flag) {
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
