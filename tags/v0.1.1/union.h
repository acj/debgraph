#ifndef UNION_H
#define UNION_H

#include "dgbinaryoperator.h"
#include "graph.h"

using namespace std;

class Union : DGBinaryOperator {
	private:
	public:
		Union(Graph &g1, Graph &g2) : DGBinaryOperator(g1, g2) { };
		~Union();
		Graph& execute();
};
#endif /* UNION_H */
