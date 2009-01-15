#include "memacct.h"

#include <iostream>

MemAcct* MemAcct::ma = 0;

MemAcct::MemAcct() {
}

MemAcct::~MemAcct() {
}

MemAcct* MemAcct::instance() {
	if (ma == 0) {
		ma = new MemAcct();
	}
	return ma;
}

void MemAcct::addReference(void *p) {
	objectList.insert(p);
}

void MemAcct::deleteReference(void *p) {
	objectList.erase(p);
}

bool MemAcct::hasReference(void *p) {
	if (objectList.find(p) != objectList.end()) {
		return true;
	}
	else {
		return false;
	}
}
