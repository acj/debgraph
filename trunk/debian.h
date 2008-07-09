#ifndef DEBIAN_H
#define DEBIAN_H

#include <string>

#include "graph.h"

using namespace std;

class DebianGraph : public Graph {
	public:
		DebianGraph(string cachedir);
		~DebianGraph();
		void freeNodes();
};

#endif /* DEBIAN_H */
