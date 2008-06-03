#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "dgoperator.h"
#include "graph.h"

using namespace std;

class Intersection : DGOperator {
	private:
		Graph l_operand;
		Graph r_operand;
		Graph result;
	protected:
	public:
		Intersection(Graph &, Graph &);
		~Intersection();
		Graph& execute();
};
#endif /* INTERSECTION_H */
