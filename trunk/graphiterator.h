#ifndef GRAPHITERATOR_H
#define GRAPHITERATOR_H

#include <iterator>
#include <map>
#include "graph.h"
#include "node.h"
#include "graph_types.h"

using namespace std;

class Graph;

class GraphIterator : public std::iterator <std::input_iterator_tag, Node> {
	private:
		//GraphIndexIteratorT graph_iter;
		Graph* graph;
		GraphIndexIteratorT graph_iter;
	protected:
		inline void setGraph(Graph *g) { graph = g; }
		inline void setIndexIterator(GraphIndexIteratorT i) { graph_iter = i; }
	public:
		friend class Graph;
		GraphIterator();
		//GraphIterator& operator= (Graph*);
		GraphIterator &operator++();
		// XXX other operator name? (pre-increment?)
		bool operator==(const GraphIterator &);
		bool operator!=(const GraphIterator &);
		Node* operator*();
};

#endif /* GRAPHITERATOR_H */
