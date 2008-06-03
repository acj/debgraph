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
		GraphIndexConstIteratorT graph_iter;
	protected:
		inline void setGraph(Graph *g) { graph = g; }
		inline void setIndexIterator(GraphIndexConstIteratorT i) { graph_iter = i; }
	public:
		friend class Graph;
		GraphIterator();
		//GraphIterator& operator= (Graph*);
		GraphIterator &operator++();
		bool operator==(const GraphIterator &) const;
		bool operator!=(const GraphIterator &) const;
		Node* operator*() const;
};

#endif /* GRAPHITERATOR_H */
