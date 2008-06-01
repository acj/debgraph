#ifndef XOR_H
#define XOR_H

#include "dgoperator.h"
#include "graph.h"

using namespace std;

class XOR : DGOperator {
	private:
		Graph &l_operand;
		Graph &r_operand;
	protected:
	public:
		XOR(Graph &, Graph &);
		~XOR();
		Graph* execute();
};
#endif /* XOR_H */
