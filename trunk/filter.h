#ifndef FILTER_H
#define FILTER_H

#include <vector>
#include "dgoperator.h"
#include "graph.h"

using namespace std;

struct FilterPair {
	string key;
	string value;
};
typedef vector< struct FilterPair > FilterProperties;
typedef FilterProperties::const_iterator FilterPropertiesIterator;
enum FilterType {
	FILTER_AND,
	FILTER_OR
};

class Filter : DGOperator {
	private:
		Graph &l_operand;
		Graph result;
		FilterProperties properties;
		FilterType filterType;
	public:
		Filter(Graph &, FilterProperties, FilterType=FILTER_AND);
		~Filter();
		void addCriterion(FilterPair);
		void setFilterProperties(FilterProperties);
		Graph& execute();
};
#endif /* FILTER_H */
