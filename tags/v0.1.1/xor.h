#ifndef XOR_H
#define XOR_H

#include "dgbinaryoperator.h"
#include "graph.h"

using namespace std;

class XOR : DGBinaryOperator {
	private:
	public:
		XOR(Graph &g1, Graph &g2) : DGBinaryOperator(g1, g2) { };
		~XOR();
		Graph& execute();
};
#endif /* XOR_H */
