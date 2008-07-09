#include "xor.h"
#include "difference.h"
#include "union.h"

// NB: Constructor defined in header file.

XOR::~XOR() {
}

Graph& XOR::execute() {
	Difference *d1 = new Difference(l_operand, r_operand);
	Difference *d2 = new Difference(r_operand, l_operand);
	Union *u = new Union(d1->execute(), d2->execute());
	Graph &result = u->execute();
	copyConsistentEdges(l_operand, r_operand, result);
	return result;
}
