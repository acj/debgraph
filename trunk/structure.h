#ifndef STRUCTURE_H
#define STRUCTURE_H

#include "testkit.h"

class Structure: public Test {
	public:
		string getName();
		bool run(Graph &g);
};

static TestKit *_tk = TestKit::instance()->addTest(new Structure());

#endif /* NULLTEST_H */
