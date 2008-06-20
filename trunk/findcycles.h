#ifndef FINDCYCLES_H
#define FINDCYCLES_H

#include <list>
#include <map>
#include <set>
#include <vector>
#include "dgunaryoperator.h"
#include "graph.h"

class FindCycles: public DGUnaryOperator {
	public:
		// Some pre-packaged sets of entities for common tasks
		enum EntityGroup {
			PRE_DEPENDS,
			DEPENDS,
			CONFLICTS
		};
	private:
		struct NodeState {
			int dfs;
			int low;
			int mark;
		};
		void initTraversalData();
		void markRecursive(Node *, int, int);
		set<Entity::EntityType> allowedEntities;
		vector<Graph> cycles;
		map<string,NodeState*> traversalData;
	protected:
		int tarjan(Graph &g, Node *n, list<Node*> *l, int *N, string dist);
		list<string> ssource;
		list<string> packages;
	public:
		vector<Graph>& getCycles();
		Graph& execute();
		FindCycles(Graph &g, set<Entity::EntityType> eTypes) : DGUnaryOperator(g)
		{
			allowedEntities = eTypes;
			initTraversalData();
		};
		FindCycles(Graph &g, EntityGroup eGroup = DEPENDS) : DGUnaryOperator(g)
		{
			switch(eGroup) {
				case PRE_DEPENDS:
					allowedEntities.insert(Entity::CONTAINS);
					allowedEntities.insert(Entity::PRE_DEPENDS);
					allowedEntities.insert(Entity::HAS_VERSION);
					break;
				case DEPENDS:
					allowedEntities.insert(Entity::CONTAINS);
					allowedEntities.insert(Entity::PRE_DEPENDS);
					allowedEntities.insert(Entity::DEPENDS);
					allowedEntities.insert(Entity::HAS_VERSION);
					break;
				case CONFLICTS:
					allowedEntities.insert(Entity::CONTAINS);
					allowedEntities.insert(Entity::PRE_DEPENDS);
					allowedEntities.insert(Entity::DEPENDS);
					allowedEntities.insert(Entity::HAS_VERSION);
					allowedEntities.insert(Entity::CONFLICTS);
					break;
				default:
					break;
			}
			initTraversalData();
		};
		~FindCycles();
};

#endif /* FINDCYCLES_H */
