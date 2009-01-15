#ifndef UNIONTEST_H
#define UNIONTEST_H

#include "testkit.h"

class UnionTest : public Test {
	public:
		bool run(Graph &g);
		inline string getName() { return "UnionTest"; }
};

static TestKit *_tk = TestKit::instance()->addTest(new UnionTest());

#endif /* UNIONTEST_H */
