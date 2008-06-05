#include <iostream>
#include "filter.h"

// NB: Constructor defined in header file.

Filter::~Filter() {
}

void Filter::addCriterion(FilterRule fRule) {
	properties.push_back(fRule);
}

void Filter::setFilterProperties(FilterProperties filterProperties) {
	properties = filterProperties;
}

Graph& Filter::execute() {
	FilterPropertiesIterator iter;
	bool node_matches = true;
	for (GraphIterator graph_iter = operand.begin(); 
			graph_iter != operand.end(); ++graph_iter) {
		// Check each property
		if (filterType == FILTER_AND) {
			node_matches = true;	
			for (iter = properties.begin(); iter != properties.end(); ++iter) {
				switch (iter->verb) {
					case CONTAINS:
					{
						const string &s = (*graph_iter)->getProperty(iter->subject);
						if (s.find(iter->object) == string::npos) {
							node_matches = false;
						}
						else {
							continue;
						}
						break;
					}
					case NCONTAINS:
					{
						const string &s = (*graph_iter)->getProperty(iter->subject);
						if (s.find(iter->object) == string::npos) {
							continue;
						}
						else {
							node_matches = false;
						}
						break;
					}
					case EQUALS:
					{
						if ((*graph_iter)->getProperty(iter->subject) == iter->object) {
							continue;
						}
						else {
							node_matches = false;
						}
						break;
					}
					case NEQUALS:
					{
						if ((*graph_iter)->getProperty(iter->subject) != iter->object) {
							continue;
						}
						else {
							node_matches = false;
						}
						break;
					}
				}
				// Do not continue the loop longer than necessary
				if (!node_matches) {
					break;
				}
			}
		}
		else {
			// FILTER_OR
			node_matches = false;
			for (iter = properties.begin(); iter != properties.end(); ++iter) {
				switch (iter->verb) {
					case CONTAINS:
					{
						const string &s = (*graph_iter)->getProperty(iter->subject);
						if (s.find(iter->object) == string::npos) {
							continue;
						}
						else {
							node_matches = true;
						}
						break;
					}
					case NCONTAINS:
					{
						const string &s = (*graph_iter)->getProperty(iter->subject);
						if (s.find(iter->object) == string::npos) {
							continue;
						}
						else {
							node_matches = true;
						}
						break;
					}
					case EQUALS:
					{
						if ((*graph_iter)->getProperty(iter->subject) == iter->object) {
							// Match!
							node_matches = true;
							break;
						}
						else {
							continue;
						}
						break;
					}
					case NEQUALS:
					{
						if ((*graph_iter)->getProperty(iter->subject) != iter->object) {
							// Match!
							node_matches = true;
						}
						break;
					}
				}
				// Don't continue the loop longer than necessary
				if (node_matches) {
					break;
				}
			}
		}
		if (node_matches) {
			result.addNode(new Node(**graph_iter), Graph::FAIL_DUP);
		}
	}
	addConsistentEdges();
	return result;
}
