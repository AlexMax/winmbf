// Emacs style mode select   -*- C++ -*-
//-----------------------------------------------------------------------------
//
//  Copyright (C) 2013 by
//  Alex Mayfield
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 
//  02111-1307, USA.
//
// DESCRIPTION:
//      Lua interface.
//
//-----------------------------------------------------------------------------

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "doomtype.h"
#include "z_zone.h"

#include "lua_events.h"
#include "lua_mobj.h"

static const luaL_Reg loadedlibs[] = {
	{"_G", luaopen_base},
	{"events", luaopen_events},
	{"mobj", luaopen_mobj},
	{NULL, NULL},
};

lua_State* lua;

/**
 * lua_Alloc implementation which uses zone memory allocator.
 */
static void* LUA_Alloc(void* ud, void* ptr, size_t osize, size_t nsize)
{
	(void)ud; (void)osize;
	if (nsize == 0) {
		free(ptr);
		return NULL;
	}
	else
		return realloc(ptr, nsize);
}

/**
 * Panic function, run if a Lua error occurs outside of a protected environment.
 */
static int LUA_Panic(lua_State *L)
{
	luai_writestringerror("PANIC: unprotected error in call to Lua API (%s)\n", lua_tostring(L, -1));
	return 0;
}

/**
 * Load game scripts from a WAD file.
 */
static void LUA_InitScripts()
{
	byte* script;
	const char* error;
	int scriptstart, scriptend, numscripts, size, i;

	scriptstart = W_GetNumForName("L_START") + 1;
	scriptend = W_GetNumForName("L_END") - 1;
	numscripts = scriptend - scriptstart + 1;

	for (i = 0;i < numscripts;i++)
	{
		size = W_LumpLength(scriptstart + i);
		if (size == 0)
			continue;

		script = (byte*)W_CacheLumpNum(scriptstart + i, PU_STATIC);
		realloc(script, size + 1);
		script[size] = 0;

		if (luaL_loadstring(lua, script) != LUA_OK)
			I_Error("Lua load error: %s", lua_tostring(lua, -1));

		if (lua_pcall(lua, 0, 0, 0) != LUA_OK)
			I_Error("Lua runtime error: %s", lua_tostring(lua, -1));
	}
}

/**
 * Initialize LUA scripts.
 */
void LUA_Init()
{
	const luaL_Reg *lib;

	lua = lua_newstate(LUA_Alloc, NULL);
	if (lua)
		lua_atpanic(lua, &LUA_Panic);

	for (lib = loadedlibs; lib->func; lib++) {
		luaL_requiref(lua, lib->name, lib->func, 1);
		lua_pop(lua, 1);
	}

	LUA_InitScripts();
}

/**
 * Close LUA state.
 */
void LUA_Free()
{
	lua_close(lua);
}