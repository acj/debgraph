#include "xor.h"
#include "difference.h"
#include "union.h"

XOR::XOR(Graph &oper1, Graph &oper2)
	: l_operand(oper1), r_operand(oper2) {
}

XOR::~XOR() {
}

Graph& XOR::execute() {
	Difference *d1 = new Difference(l_operand, r_operand);
	Difference *d2 = new Difference(r_operand, l_operand);
	Union *u = new Union(d1->execute(), d2->execute());
	return u->execute();
}
