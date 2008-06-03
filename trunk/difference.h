#ifndef DIFFERENCE_H
#define DIFFERENCE_H

#include "dgoperator.h"
#include "graph.h"

using namespace std;

class Difference : DGOperator {
	private:
		Graph &l_operand;
		Graph &r_operand;
		Graph result;
	protected:
	public:
		Difference(Graph &, Graph &);
		~Difference();
		Graph& execute();
};
#endif /* DIFFERENCE_H */
