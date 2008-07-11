#include <stdio.h>
#include <string.h>
extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "libdebgraph.h"
}

int main (void) {
	char buff[256];
	int error;
	lua_State *L = lua_open();   /* opens Lua */
	luaL_openlibs(L);
	// For Lua 5.0
	/*
	luaopen_base(L);			// opens the basic library
	luaopen_table(L);			// opens the table library
	luaopen_io(L);				// opens the I/O library
	luaopen_string(L);			// opens the string library
	luaopen_math(L);			// opens the math library
	*/
	// For Lua 5.1
	luaopen_libdebgraph(L);

	printf("> ");
	while (fgets(buff, sizeof(buff), stdin) != NULL) {
		error = luaL_loadbuffer(L, buff, strlen(buff), "line") ||
			lua_pcall(L, 0, 0, 0);
		if (error) {
			fprintf(stderr, "%s\n", lua_tostring(L, -1));
			lua_pop(L, 1);  /* pop error message from the stack */
		}
		printf("> ");
	}

	lua_close(L);
	return 0;
}
