#ifndef NULLTEST_H
#define NULLTEST_H

#include "testkit.h"

class NullTest: public Test {
	public:
		bool run(Graph &g);
		string getName();
};

static TestKit *_tk = TestKit::instance()->addTest(new NullTest());

#endif /* NULLTEST_H */
