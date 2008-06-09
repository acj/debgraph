#ifndef INTERSECTIONTEST_H
#define INTERSECTIONTEST_H

#include "testkit.h"

class IntersectionTest : public Test {
	public:
		bool run(Graph &g);
		inline string getName() { return "IntersectionTest"; }
};

static TestKit *_tk = TestKit::instance()->addTest(new IntersectionTest());

#endif /* INTERSECTIONTEST_H */
