#ifndef XORTEST_H
#define XORTEST_H

#include "testkit.h"

class XORTest : public Test {
	public:
		bool run(Graph &g);
		inline string getName() { return "XORTest"; }
};

static TestKit *_tk = TestKit::instance()->addTest(new XORTest());

#endif /* XORTEST_H */
