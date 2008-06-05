#ifndef FILTER_H
#define FILTER_H

#include <vector>
#include "dgoperator.h"
#include "graph.h"

using namespace std;

enum FilterType {
	FILTER_AND,
	FILTER_OR
};
enum FilterVerb {
	CONTAINS,
	NCONTAINS,
	EQUALS,
	NEQUALS
};
struct FilterRule {
	string subject;
	FilterVerb verb;
	string object;
};
typedef vector< struct FilterRule > FilterProperties;
typedef FilterProperties::const_iterator FilterPropertiesIterator;

class Filter : DGOperator {
	private:
		Graph &l_operand;
		Graph result;
		FilterProperties properties;
		FilterType filterType;
	public:
		Filter(Graph &, FilterProperties, FilterType=FILTER_AND);
		~Filter();
		void addCriterion(FilterRule);
		void setFilterProperties(FilterProperties);
		Graph& execute();
};
#endif /* FILTER_H */
