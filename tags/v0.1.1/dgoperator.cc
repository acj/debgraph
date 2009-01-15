#include "dgoperator.h"

DGOperator::DGOperator() {
}

DGOperator::~DGOperator() {
}

void DGOperator::registerObject(void *p) {
	MemAcct *ma = MemAcct::instance();
	ma->addReference(p);
}
