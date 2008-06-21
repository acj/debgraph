#ifndef CYCLICTESTTEST_H
#define CYCLICTESTTEST_H

#include "testkit.h"

class CyclicTest : public Test {
	public:
		bool run(Graph &g);
		inline string getName() { return "CyclicTest"; }
};

static TestKit *_tk = TestKit::instance()->addTest(new CyclicTest());

#endif /* CYCLICTESTTEST_H */
