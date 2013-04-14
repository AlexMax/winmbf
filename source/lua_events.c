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
//      Lua event handler.  Based loosely on Node.js's events library.
//
//-----------------------------------------------------------------------------

#include "lua.h"
#include "lauxlib.h"

#include "lua_main.h"
#include "lua_events.h"

static const char* registrykey = "eventslib";

static const char* scripteventnames[MAXSEVENTS] = {
	"open", "enter", "respawn", "death", "disconnect"
};

/**
 * Implement `events.addListener(event, listener)`.
 */
static int eventslib_addListener(lua_State* L)
{
	const char* event;

	luaL_checkstring(L, 1);
	luaL_checktype(L, 2, LUA_TFUNCTION);

	// Get the table of event types out of the registry.
	lua_pushlightuserdata(L, (void*)&registrykey);
	lua_gettable(L, LUA_REGISTRYINDEX);
	if (lua_type(L, -1) != LUA_TTABLE)
	{
		// We don't have an event table yet.  Create one.
		lua_pop(L, 1);
		lua_pushlightuserdata(L, (void*)&registrykey);
		lua_createtable(L, 0, 1);
		lua_settable(L, LUA_REGISTRYINDEX);

		// Get the newly-minted table of event types.
		lua_pushlightuserdata(L, (void*)&registrykey);
		lua_gettable(L, LUA_REGISTRYINDEX);
	}

	// Get the table of events for the given event type.
	lua_pushvalue(L, 1); // [events][event]
	lua_gettable(L, -2); // [events][table?]
	if (lua_type(L, -1) != LUA_TTABLE)
	{
		// We don't have a table of events for this event type yet.  Create one.
		lua_pop(L, 1);
		lua_pushvalue(L, 1);
		lua_createtable(L, 1, 0);
		lua_settable(L, -3);

		// Get the newly-minted table of events
		lua_pushvalue(L, 1);
		lua_gettable(L, -2);
	}

	// Add one to the length, this is the key we'll put the listener in.
	lua_len(L, -1);
	lua_pushinteger(L, 1);
	lua_arith(L, LUA_OPADD);

	// Add listener to the table of event listeners.
	lua_pushvalue(L, 2);
	lua_settable(L, -3);

	return 0;
}

/**
 * Implement `events.removeListener(event, listener)`.
 */
static int eventslib_removeListener(lua_State* L)
{
	luaL_checkstring(L, 1);
	luaL_checktype(L, 2, LUA_TFUNCTION);

	return 0;
}

/**
 * Implement `events.removeAllListener(event)`.
 */
static int eventslib_removeAllListeners(lua_State* L)
{
	luaL_checkstring(L, 1);

	return 0;
}

/**
 * Implement `events.listeners(event)`.
 */
static int eventslib_listeners(lua_State* L)
{
	luaL_checkstring(L, 1);

	return 0;
}

/**
 * Implement `events.emit(event, args...)`.
 */
static int eventslib_emit(lua_State* L)
{
	luaL_checkstring(L, 1);

	// Get the table of event types out of the registry.
	lua_pushlightuserdata(L, (void*)&registrykey);
	lua_gettable(L, LUA_REGISTRYINDEX);
	if (lua_type(L, -1) != LUA_TTABLE)
		return 0;

	// Get the table of events out of the table of event types.
	lua_pushvalue(L, 1);
	lua_gettable(L, -2);
	if (lua_type(L, -1) != LUA_TTABLE)
		return 0;

	// Loop through the table of events, executing every function in the table.
	lua_pushnil(L);
	while (lua_next(L, -2) != 0)
	{
		lua_pcall(L, 0, 1, 0);
		lua_pop(L, 1);
	}

	return 0;
}

static const luaL_Reg eventslib[] =
{
	{"addListener", eventslib_addListener},
	{"removeListener", eventslib_removeListener},
	{"removeAllListeners", eventslib_removeAllListeners},
	{"listeners", eventslib_listeners},
	{"emit", eventslib_emit},
	{NULL, NULL},
};

int luaopen_events(lua_State* L)
{
	luaL_newlib(L, eventslib);
	return 1;
}

void LUA_EmitEvent(scriptevent_t event, char* lumpname)
{
	lua_getglobal(lua, "events");
	lua_getfield(lua, -1, "emit");
	lua_pushstring(lua, scripteventnames[event]);
	lua_pcall(lua, 1, 0, 0);
}
