#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <set>

#include "debian.h"
#include "difference.h"
#include "filter.h"
#include "findcycles.h"
#include "finddeps.h"
#include "findreversedeps.h"
#include "intersection.h"
#include "union.h"
#include "xor.h"

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

static MemAcct *memAcct = MemAcct::instance();

/* Create/set an integer field in a Lua table */
static void setIntField(lua_State *L, const char *index, int value) {
	lua_pushstring(L, index);
	lua_pushnumber(L, value);
	lua_settable(L, -3);
}

/* Create/set a string field in a Lua table */
static void setStringField(lua_State *L, const char *index, const char *value) {
	lua_pushstring(L, index);
	lua_pushstring(L, value);
	lua_settable(L, -3);
}

/* Debugging function that prints the contents of the Lua stack */
static int stackDump (lua_State *L) {
	int i;
	int top = lua_gettop(L);
	for (i = 1; i <= top; i++) {/* repeat for each level */
		int t = lua_type(L, i);
		switch (t) {
	
		case LUA_TSTRING:/* strings */
			printf("`%s'", lua_tostring(L, i));
			break;
	
		case LUA_TBOOLEAN:/* booleans */
			printf(lua_toboolean(L, i) ? "true" : "false");
			break;
	
		case LUA_TNUMBER:/* numbers */
			printf("%g", lua_tonumber(L, i));
			break;
	
		default:/* other values */
			printf("%s", lua_typename(L, t));
			break;
	
		}
		printf("\n");/* put a separator */
	}
	printf("\n");/* end the listing */
	return 0;
}

/* Pop a "Graph" object from the Lua stack */
Graph* popGraph(lua_State *L) {
	if (lua_type(L, -1) == LUA_TTABLE) {
		lua_pushstring(L, "__ptr");
		lua_gettable(L, -2);
		if (lua_type(L, -1) == LUA_TNUMBER) {
			size_t gPtr = lua_tonumber(L, -1);
			lua_pop(L, 2); // pop the Graph/table off the stack
			if (memAcct->hasReference((void *)gPtr)) {
				return (Graph *)gPtr;
			}
		}
		else {
			lua_pushstring(L, "Invalid pointer value");
			lua_error(L);
		}
	}
	// The object on top of the stack is not a Graph
	return 0;
}

void pushGraphAsTable(lua_State *L, Graph *g) {
	lua_newtable(L);
	setIntField(L, "size", g->size());	/* number of nodes */
	setIntField(L, "__ptr", (size_t)g);	/* pointer to Graph object */
	// TODO: __ptr needs to be hidden from Lua code somehow
}

/* Push nodes in the Graph onto the Lua stack as an array.
 *
 * NB: This function does not create a new Lua table.
 */
int pushNodesAsArray(lua_State *L, Graph *g) {
	int idx = 1;
	for (GraphIterator i = g->begin(); i != g->end(); ++i) {
		lua_pushnumber(L, idx);
		if ((*i)->hasProperty("Package")) {
			lua_pushstring(L, (*i)->getProperty("Package").c_str());
		}
		else {
			lua_pushstring(L, "(none)");
		}
		lua_settable(L, -3);
		++idx;
	}
	return 1;
}

static int getNodes(lua_State *L) {
	Graph *g = popGraph(L);
	if (g == 0) {
		lua_pushstring(L, "Could not get nodes from graph");
		lua_error(L);
	} else if (!memAcct->hasReference(g)) {
		lua_pushstring(L, "Invalid pointer");
		lua_error(L);
	}
	lua_newtable(L);
	pushNodesAsArray(L, g);
	return 1;
}

static int loadPackages(lua_State *L) {
	const char *pkgPath;
	if (lua_type(L, -1) != LUA_TSTRING) {
		lua_pushstring(L, "Path must be a string");
		lua_error(L);
	} else {
		pkgPath = lua_tostring(L, -1);
		DebianGraph *g = new DebianGraph(pkgPath);
		memAcct->addReference(g);
		pushGraphAsTable(L, g);
		lua_setglobal(L, "g");
	}
	return 0;
}

static int operDifference(lua_State *L) {
	Graph *g2 = popGraph(L);
	Graph *g1 = popGraph(L);
	if (g1 == 0 || g2 == 0) {
		return 0;
	}
	else {
		Difference *d = new Difference(*g1, *g2);
		Graph &result = d->execute();
		pushGraphAsTable(L, &result);
		return 1;
	}
}

static int operFilter(lua_State *L) {
	Graph *g1 = popGraph(L);
	if (g1 == 0) {
		return 0;
	}
	else {
		FilterProperties fProperties;
		FilterRule fRuleSection = { string("Section"), EQUALS, string("games") };
		fProperties.push_back(fRuleSection);
		Filter f = Filter(*g1, fProperties, FILTER_AND);
		Graph &result = f.execute();
		pushGraphAsTable(L, &result);
		return 1;
	}
}

static int operFindCycles(lua_State *L) {
	Graph *g = popGraph(L);
	if (g == 0) {
		return 0;
	}
	// TODO: Parameterize the starting node (passed from Lua)
	FindCycles fc(*g, FindCycles::DEPENDS, string("Release:unstable"), false);
	fc.execute();
	vector<Graph*> &cycles = fc.getCycles();
	size_t cycleCount = cycles.size();
	// This operation can consume a large amount of stack space.  We
	// need to ensure that there is a slot available for each cycle
	// (stored as a table).  There must be three additional slots available
	// to store the inner table and one key/value pair.
	lua_checkstack(L, cycleCount + 3);
	lua_newtable(L);
	for (size_t i=0; i<cycleCount; ++i) {
		lua_newtable(L);
		lua_pushnumber(L, i);
		lua_pushvalue(L, -2);
		lua_settable(L, -4);
		pushNodesAsArray(L, cycles[i]);
		lua_pop(L, 1);
	}
	return 1;
}

static int operFindDeps(lua_State *L) {
	// Second argument (node name)
	if (lua_type(L, -1) != LUA_TSTRING) {
		lua_pushstring(L, "Arg #2: string expected");
		lua_error(L);
	}
	const char *nodeName = lua_tostring(L, -1);
	lua_pop(L, 1);
	// First argument (graph)
	Graph *g1 = popGraph(L);
	if (g1 == 0) {
		lua_pushstring(L, "Arg #1: Not a graph or the graph does not exist");
		lua_error(L);
	} else if (!g1->hasNode(nodeName)) {
		string err("Could not locate node `" + string(nodeName) + "'");
		lua_pushstring(L, err.c_str());
		lua_error(L);
		return 0;
	}
	Node *n = g1->findNode(string(nodeName));
	FindDeps *fDeps = new FindDeps(*g1, n);
	Graph &result = fDeps->execute();
	pushGraphAsTable(L, &result);
	return 1;
}

static int operFindReverseDeps(lua_State *L) {
	// Second argument (node name)
	if (lua_type(L, -1) != LUA_TSTRING) {
		lua_pushstring(L, "Arg #2: Not a string");
		lua_error(L);
	}
	const char *nodeName = lua_tostring(L, -1);
	lua_pop(L, 1);
	// First argument (graph)
	Graph *g1 = popGraph(L);
	if (g1 == 0) {
		lua_pushstring(L, "Arg #1: Not a graph or the graph does not exist");
		lua_error(L);
	} else if (!g1->hasNode(nodeName)) {
		string err("Could not locate node `" + string(nodeName) + "'");
		lua_pushstring(L, err.c_str());
		lua_error(L);
	}
	Node *n = g1->findNode(string(nodeName));
	FindReverseDeps *fRDeps = new FindReverseDeps(*g1, n);
	Graph &result = fRDeps->execute();
	pushGraphAsTable(L, &result);
	return 1;
}

static int operIntersection(lua_State *L) {
	Graph *g2 = popGraph(L);
	Graph *g1 = popGraph(L);
	if (g1 == 0 || g2 == 0) {
		return 0;
	}
	else {
		Intersection *i = new Intersection(*g1, *g2);
		Graph &result = i->execute();
		pushGraphAsTable(L, &result);
		return 1;
	}
}

static int operUnion(lua_State *L) {
	Graph *g2 = popGraph(L);
	Graph *g1 = popGraph(L);
	if (g1 == 0 || g2 == 0) {
		return 0;
	}
	else {
		Union *u = new Union(*g1, *g2);
		Graph &result = u->execute();
		pushGraphAsTable(L, &result);
		return 1;
	}
}

static int operXOR(lua_State *L) {
	Graph *g2 = popGraph(L);
	Graph *g1 = popGraph(L);
	if (g1 == 0 || g2 == 0) {
		return 0;
	}
	else {
		XOR *x = new XOR(*g1, *g2);
		Graph &result = x->execute();
		pushGraphAsTable(L, &result);
		return 1;
	}
}

static const struct luaL_reg libdebgraph [] = {
	{"LoadPackages", loadPackages},
	{"GetNodes", getNodes},
	{"Difference", operDifference},
	{"FindDeps", operFindDeps},
	{"FindRevDeps", operFindReverseDeps},
	{"Filter", operFilter},
	{"FindCycles", operFindCycles},
	{"Intersection", operIntersection},
	{"Union", operUnion},
	{"XOR", operXOR},
	{NULL, NULL} /* sentinel */
};

extern "C"
{
int luaopen_libdebgraph(lua_State *L) {
	luaL_openlib(L, "libdebgraph", libdebgraph, 0);
	/* operators */
	lua_register(L, "Difference", operDifference);
	lua_register(L, "FindDeps", operFindDeps);
	lua_register(L, "FindRevDeps", operFindReverseDeps);
	lua_register(L, "Filter", operFilter);
	lua_register(L, "FindCycles", operFindCycles);
	lua_register(L, "Intersection", operIntersection);
	lua_register(L, "Union", operUnion);
	lua_register(L, "XOR", operXOR);
	/* support routines */
	lua_register(L, "GetNodes", getNodes);
	lua_register(L, "LoadPackages", loadPackages);
	lua_register(L, "stackDump", stackDump);
	return 1;
}
}
