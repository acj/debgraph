#include <iostream>
#include <stdint.h>
#include "edge.h"
#include "node.h"

Edge::Edge() {
	to = NULL;
	from = NULL;
}

Edge::~Edge() {
}

const Node *Edge::getFromNode() const {
	return from;
}

const Node *Edge::getToNode() const {
	return to;
}

void Edge::setFromNode(Node *n) {
	from = n;
}

void Edge::setToNode(Node *n) {
	to = n;
}
