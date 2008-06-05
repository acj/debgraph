#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "dgbinaryoperator.h"
#include "graph.h"

using namespace std;

class Intersection : DGBinaryOperator {
	private:
	public:
		Intersection(Graph &g1, Graph &g2) : DGBinaryOperator(g1, g2) { };
		~Intersection();
		Graph& execute();
};
#endif /* INTERSECTION_H */
