#ifndef DIFFERENCE_H
#define DIFFERENCE_H

#include "dgbinaryoperator.h"
#include "graph.h"

using namespace std;

class Difference : DGBinaryOperator {
	private:
	public:
		Difference(Graph &l_operand, Graph &r_operand) : DGBinaryOperator(l_operand, r_operand) { };
		~Difference();
		Graph& execute();
};
#endif /* DIFFERENCE_H */
