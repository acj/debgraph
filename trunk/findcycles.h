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
		set<Entity::EntityType> allowedEntities;
		vector<Graph> cycles;
		bool discoverAllNodes;
		string startNodeId;
		map<string,NodeState*> traversalData;

		void initTraversalData();
		void markRecursive(Node *, int, int);
	protected:
		int tarjan(Graph&, Node*, list<Node*>*, int*);
	public:
		vector<Graph>& getCycles();
		Graph& execute();
		FindCycles(Graph &g, set<Entity::EntityType> eTypes, string startNode, 
				bool discoverAllNodesFlag=false) : DGUnaryOperator(g)
		{
			allowedEntities = eTypes;
			discoverAllNodes = discoverAllNodesFlag;
			initTraversalData();
			startNodeId = startNode;
		};
		FindCycles(Graph &g, EntityGroup eGroup, string startNode, 
				bool discoverAllNodesFlag) : DGUnaryOperator(g)
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
			discoverAllNodes = discoverAllNodesFlag;
			initTraversalData();
			startNodeId = startNode;
		};
		FindCycles(Graph &g, EntityGroup eGroup) : DGUnaryOperator(g)
		{
			Node *n = *(g.begin());
			FindCycles(g, eGroup, n->getId(), true);
		};
		~FindCycles();
};

#endif /* FINDCYCLES_H */
