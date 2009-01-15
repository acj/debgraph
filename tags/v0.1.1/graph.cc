#include <assert.h>
#include <iostream>
#include "graph.h"

Graph::Graph() {
}

Graph::Graph(const Graph &g) {
	clone(g);
}

Graph::~Graph() {
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
		addNode(*iter, FAIL_DUP); 
	}
	// Fix up the (outbound) edges
	string oldNearNodeId;
	Node *oldNearNode, *oldFarNode, *newNearNode, *newFarNode;
	for (GraphIterator gIter = g.begin(); 
			gIter != g.end(); 
			++gIter) {
		oldNearNode = *gIter;
		oldNearNodeId = oldNearNode->getId();
		newNearNode = findNode(oldNearNodeId);
		if (newNearNode == NULL) {
			throw("Matching new node is missing");
		}
		EdgeSet &outEdges = const_cast<Graph&>(g).getOutEdges(oldNearNode);
		for (EdgeSetIterator eIter = outEdges.begin();
				eIter != outEdges.end();
				++eIter) {
			oldFarNode = (Node *)(*eIter)->getToNode();
			newFarNode = nodeIndex.find(oldFarNode->getId())->second;
			if (newFarNode == NULL) {
				cout << "Can't find node " << oldFarNode->getId() << endl;
			}
			assert(newNearNode != NULL && newFarNode != NULL);
			createEdge(newNearNode, newFarNode, (*eIter)->getType(), Edge::IGNORE_DUP);
		}
	}
}

Edge* Graph::createEdge(Node *fromNode, Node *toNode, Entity::EntityType type, 
		Edge::CreateFlag flag) {
	if (!this->hasNode(fromNode->getId())) {
		throw("Could not find Node object in index");
		return NULL;
	}
	if (flag == Edge::IGNORE_DUP) {
		EdgeSet &edgeSet = *outEdges[fromNode];
		EdgeSetIterator it;
		for (it = edgeSet.begin(); it != edgeSet.end(); ++it) {
			if (((*it)->getType() == type) 
				&& ((*it)->getToNode() == toNode)) {
				return NULL;
			}
		}
	}
	// ok, add the edge
	Edge *e = new Edge();
	e->setFromNode(fromNode);
	e->setToNode(toNode);
	e->setType(type);
	// Add to "from" node's list
	EdgeSet &edgeSetFrom = getOutEdges(fromNode);
	edgeSetFrom.insert(e);
	// Add to "to" node's list
	EdgeSet &edgeSetTo = getInEdges(toNode);
	edgeSetTo.insert(e);
	return e;
}

void Graph::deleteEdge(Node *from, Node *to, Entity::EntityType eType) {
	EdgeSet &fromOutEdges = *outEdges[from];
	EdgeSet &toInEdges = *inEdges[to];
	for (EdgeSetIterator eIter = fromOutEdges.begin(); 
			eIter != fromOutEdges.end(); 
			++eIter) {
		if ((*eIter)->getToNode() == to && (*eIter)->getType() == eType) {
			Edge *e = *eIter;
			fromOutEdges.erase(eIter);
			toInEdges.erase(e);
			delete e;
		}
	}
}

bool Graph::hasEdge(Node *from, Node *to, Entity::EntityType eType) {
	EdgeSet &fromOutEdges = *outEdges[from];
	for (EdgeSetIterator i = fromOutEdges.begin(); 
			i != fromOutEdges.end();
			++i) {
		if ((*i)->getFromNode() == from 
				&& (*i)->getToNode() == to
				&& (*i)->getType() == eType) {
			return true;
		}
	}
	return false;
}

Node* Graph::addNode(Node *node, AddFlag flag) {
	NodeIndexT::iterator i = nodeIndex.find(node->getId());
	if (i != nodeIndex.end()) {
		if (flag == FAIL_DUP) {
			throw "duplicate node";
		}
		else if (flag == DISCARD_DUP) {
			return i->second;
		}
		/* ignore_dup means we insert anyway */
	}
	nodeIndex.insert(pair<string, Node*>(node->getId(), node));
	inEdges.insert(pair<Node*,EdgeSet*>(node, new EdgeSet()));
	outEdges.insert(pair<Node*,EdgeSet*>(node, new EdgeSet()));
	return node;
}

void Graph::deleteNode(const string &id) {
	NodeIndexIteratorT i = nodeIndex.find(id);
	if (i != nodeIndex.end()) {
		nodeIndex.erase(id);
	}
}

Node* Graph::findNode(const string &id) const {
	NodeIndexConstIteratorT i = nodeIndex.find(id);
	if (i != nodeIndex.end()) {
		return i->second;
	}
	return NULL;
}

void Graph::freeEdges() {
	map<Node*,EdgeSet*>::iterator i;	
	for (i = inEdges.begin(); i != inEdges.end(); ++i) {
		EdgeSet &edgeSet = *inEdges[(*i).first];
		for (EdgeSetIterator edgeIter=edgeSet.begin();
				edgeIter != edgeSet.end(); ++edgeIter) {
			delete(*edgeIter);
		}
	}
	for (i = outEdges.begin(); i != outEdges.end(); ++i) {
		EdgeSet &edgeSet = *outEdges[(*i).first];
		for (EdgeSetIterator edgeIter=edgeSet.begin();
				edgeIter != edgeSet.end(); ++edgeIter) {
			delete(*edgeIter);
		}
	}
}

EdgeSet& Graph::getInEdges(Node *n) {
	if (!hasNode(n->getId())) {
		throw("Node not found");
	}
	return *inEdges[n];
}

EdgeSet& Graph::getOutEdges(Node *n) {
	if (!hasNode(n->getId())) {
		throw("Node not found");
	}
	return *outEdges[n];
}

bool Graph::hasNode(const string &id) const {
	NodeIndexConstIteratorT i = nodeIndex.find(id);
	return (i != nodeIndex.end());
}

void Graph::mergePackageVersions() {
	Node *nameNode, *verNode, *otherNode;
	Edge *copyEdge;
	EdgeSetIterator nameEdgeIter, verEdgeIter;
	list<Node*> nodesToRemove;
	list<Edge*> edgesToRemove;
	for (GraphIterator i = begin(); i != end(); ++i) {
		if ((*i)->getType() == Entity::BINARYNAME) {
			nameNode = *i;
			EdgeSet &nameOutEdges = getOutEdges(nameNode);
			for (nameEdgeIter = nameOutEdges.begin(); 
					nameEdgeIter != nameOutEdges.end(); 
					++nameEdgeIter) {
				if ((*nameEdgeIter)->getType() == Entity::HAS_VERSION) {
					copyEdge = *nameEdgeIter;
					verNode = const_cast<Node*>(copyEdge->getToNode());
					EdgeSet &verOutEdges = getOutEdges(verNode);
					for (verEdgeIter = verOutEdges.begin();
							verEdgeIter != verOutEdges.end();
							++verEdgeIter) {
						otherNode = const_cast<Node*>((*verEdgeIter)->getToNode());
						createEdge(nameNode, otherNode, 
								(*verEdgeIter)->getType(), Edge::IGNORE_DUP);
						edgesToRemove.push_back(*nameEdgeIter);
						edgesToRemove.push_back(*verEdgeIter);
					}
					nodesToRemove.push_back(verNode);
				}
			}
		}
	}
	// Clean up
	for (list<Edge*>::iterator edgeIter = edgesToRemove.begin();
			edgeIter != edgesToRemove.end();
			++edgeIter) {
		deleteEdge((Node *)(*edgeIter)->getFromNode(), 
				(Node *)(*edgeIter)->getToNode(), 
				(*edgeIter)->getType());
	}
	for (list<Node*>::iterator nodeIter = nodesToRemove.begin();
			nodeIter != nodesToRemove.end();
			++nodeIter) {
		deleteNode((*nodeIter)->getId());
	}
}

int Graph::size() const {
	return nodeIndex.size();
}

NodeIndexT& Graph::getIndex() {
	return nodeIndex;
}

/* Iterator-related members */
GraphIterator &Graph::begin() const {
	NodeIndexConstIteratorT index_iter = nodeIndex.begin();
	GraphIterator *iter = new GraphIterator();
	iter->setIndexIterator(index_iter);
	return *iter;
}

GraphIterator &Graph::end() const {
	NodeIndexConstIteratorT index_iter = nodeIndex.end();
	GraphIterator *iter = new GraphIterator();
	iter->setIndexIterator(index_iter);
	return *iter;
}

string Graph::toGraphviz() {
	string output("digraph \"DebGraph Output\" {\n");
	output += "nodesep=0.1\n";
	const size_t PTRWIDTH = 21;
	for (GraphIterator gIter = this->begin(); gIter != this->end(); ++gIter) {
		Node *n = (*gIter);
		char node_id_salt[PTRWIDTH];
		sprintf(node_id_salt, "%u", (size_t)n);
		output += "node" + string(node_id_salt) + "[";
		if (n->getProperty("Package") == "" 
				&& n->getType() == Entity::OR) {
			output += "shape=circle, label=\"OR\" ";
		}
		else {
			output += "label=\"" + n->getProperty("Package")
				+ "\\n" + n->getProperty("Version")
				+ "\\n" + n->getProperty("Architecture");
		}
		output += "\"];\n";
		EdgeSet &edgeSet = getOutEdges(n);
		char source_gid[PTRWIDTH], sink_gid[PTRWIDTH];
		for (EdgeSetIterator i = edgeSet.begin(); i != edgeSet.end(); ++i) {
			Edge *e = *i;
			sprintf(source_gid, "%u", (size_t)e->getFromNode());
			sprintf(sink_gid, "%u", (size_t)e->getToNode());
			output += "\t node";
			output += source_gid + string(" -> node") + sink_gid;
			output += " [ label = \"" + e->getTypeString() + "\" ];\n";
		}
	}
	output += "}";
	return output;
}
