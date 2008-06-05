#ifndef FILTER_H
#define FILTER_H

#include <vector>
#include "dgunaryoperator.h"
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

class Filter : DGUnaryOperator {
	private:
		FilterProperties properties;
		FilterType filterType;
	public:
		Filter(Graph &g, FilterProperties fp, FilterType ft=FILTER_AND) 
			: DGUnaryOperator(g) 
		{
			properties = fp;
			filterType = ft;
		};
		~Filter();
		void addCriterion(FilterRule);
		void setFilterProperties(FilterProperties);
		Graph& execute();
};
#endif /* FILTER_H */
