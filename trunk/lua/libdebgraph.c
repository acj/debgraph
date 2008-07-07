#include "debian.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
}

static DebianGraph *g;

/*
static int newLuaGraph(lua_State *L) {
	size_t nbytes = sizeof(Node*);
	Node *n = (Node *)lua_newuserdata(L, nbytes);

}
*/

static void setStringField(lua_State *L, const char *index, const char *value) {
	lua_pushstring(L, index);
	lua_pushstring(L, value);
	lua_settable(L, -3);
}

static void setIntField(lua_State *L, const char *index, int value) {
	lua_pushstring(L, index);
	lua_pushnumber(L, value);
	lua_settable(L, -3);
}

static int getNodes(lua_State *L) {
	Graph *g;
	lua_pushnil(L);
	while (lua_next(L, -2) != 0) {
		if (strcmp(lua_typename(L, lua_type(L, -2)), "string") == 0
			&& strcmp(lua_typename(L, lua_type(L, -1)), "number") == 0
			&& strcmp(lua_tostring(L, -2), "ptr") == 0) {
			size_t gPtr = lua_tonumber(L, -1);
			g = (Graph *)gPtr;
		}
		lua_pop(L, 1);
	}
	lua_newtable(L);
	int idx = 1;
	for (GraphIterator i = g->begin(); i != g->end(); ++i, ++idx) {
		lua_pushstring(L, (*i)->getProperty("Package").c_str());
		lua_rawseti(L, -2, idx);
	}
	return 1;
}

static int loadPackages(lua_State *L) {
	// FIXME: Need to use the directory passed from Lua
	char src[] = "cache";
	g = new DebianGraph(src);
	lua_newtable(L);
	setStringField(L, "name", "Debian Packages");
	setStringField(L, "sourcepath", src);
	setIntField(L, "size", g->size());
	setIntField(L, "ptr", (size_t)g);
	lua_setglobal(L, "g");
	return 0;
}

static const struct luaL_reg libdebgraph [] = {
	{"loadPackages", loadPackages},
	{"getNodes", getNodes},
	{NULL, NULL} /* sentinel */
};

extern "C"
{
int luaopen_libdebgraph(lua_State *L) {
	luaL_openlib(L, "libdebgraph", libdebgraph, 0);
	//lua_register(L, "loadPackages", loadPackages);
	return 1;
}
}
