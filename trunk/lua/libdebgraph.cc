#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "debian.h"
#include "findcycles.h"

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

static DebianGraph *g;

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
		size_t gPtr = lua_tonumber(L, -1);
		lua_pop(L, 1); // pop the Graph/table off the stack
		return (Graph *)gPtr;
	}
	else {
		return 0;
	}
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
		lua_pushstring(L, (*i)->getProperty("Package").c_str());
		lua_settable(L, -3);
		++idx;
	}
	return 1;
}

static int getNodes(lua_State *L) {
	Graph *g = popGraph(L);
	if (g == 0) {
		// TODO: Return a Lua error
		return 0;
	}
	pushNodesAsArray(L, g);
	return 1;
}

static int loadPackages(lua_State *L) {
	const char *pkgPath;
	if (lua_type(L, -1) != LUA_TSTRING) {
		// TODO: This should be a Lua error
		fprintf(stderr, "Path must be a string\n");
	} else {
		pkgPath = lua_tostring(L, -1);
		g = new DebianGraph(pkgPath);
		pushGraphAsTable(L, g);
		lua_setglobal(L, "g");
	}
	return 0;
}

static int operFindCycles(lua_State *L) {
	Graph *g = popGraph(L);
	if (g == 0) {
		return 0;
	}
	// TODO: Parameterize the starting node (passed from Lua)
	FindCycles fc(*g, FindCycles::DEPENDS, string("Release:unstable"), false);
	fc.execute();
	size_t cycleCount = fc.getCycles().size();
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
		Graph &gCycle = fc.getCycles()[i];
		pushNodesAsArray(L, &gCycle);
		lua_pop(L, 1);
	}
	return 1;
}

static const struct luaL_reg libdebgraph [] = {
	{"LoadPackages", loadPackages},
	{"GetNodes", getNodes},
	{NULL, NULL} /* sentinel */
};

extern "C"
{
int luaopen_libdebgraph(lua_State *L) {
	luaL_openlib(L, "libdebgraph", libdebgraph, 0);
	lua_register(L, "FindCycles", operFindCycles);
	lua_register(L, "GetNodes", getNodes);
	lua_register(L, "LoadPackages", loadPackages);
	lua_register(L, "stackDump", stackDump);
	return 1;
}
}
