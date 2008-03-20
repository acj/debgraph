#ifndef GRAPHWALKER_H
#define GRAPHWALKER_H

#include "node.h"

using namespace std;

class GraphWalker {
	private:
		Node *current;
	protected: 
		void mark(Node &n);
		void finish();
		virtual bool acceptOutEdge(Edge &e);
		virtual bool acceptInEdge(Edge &e);
		virtual bool acceptNode(Edge &e);
		virtual ~GraphWalker();
	public:
		void start(Node &n);
		Node *next();
		// XXX also needs a run on an iterator as well as an iterator-like
		// interface
};

#endif /* GRAPHWALKER_H */
