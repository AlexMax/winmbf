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
//      Lua mobj interface.
//
//-----------------------------------------------------------------------------

#include "lua.h"
#include "lauxlib.h"

#include "p_mobj.h"

static int mobjlib_spawn(lua_State* L)
{
  mobjinfo_t* mobj;
  double x, y, z;

  mobj = luaL_checkudata(L, 1, "Doom.mobj");

  x = luaL_checknumber(L, 2);
  y = luaL_checknumber(L, 3);
  z = luaL_checknumber(L, 4);

  return 0;
}

static const luaL_Reg mobjlib_methods[] =
{
  {"spawn", mobjlib_spawn},
  {NULL, NULL},
};

static int mobjlib_new(lua_State* L)
{
  int success, doomednum, spawnhealth, reactiontime, painchance, speed, mass,
      damage;
  double radius, height;
  mobjinfo_t* mobj;

  // We must have a constructor table with appropriate types.
  // [AM] TODO: Support subclassing and empty.
  luaL_checktype(L, 1, LUA_TTABLE);

  // doomednum
  lua_getfield(L, -1, "doomednum");
  doomednum = lua_tointegerx(L, -1, &success);
  if (!success)
    doomednum = -1; // default
  lua_pop(L, 1);

  // spawnhealth
  lua_getfield(L, -1, "spawnhealth");
  spawnhealth = lua_tointegerx(L, -1, &success);
  if (!success)
    spawnhealth = 1000; // default
  lua_pop(L, 1);

  // reactiontime
  lua_getfield(L, -1, "reactiontime");
  reactiontime = lua_tointegerx(L, -1, &success);
  if (!success)
    reactiontime = 8; // default
  lua_pop(L, 1);

  // painchance
  lua_getfield(L, -1, "painchance");
  painchance = lua_tointegerx(L, -1, &success);
  if (!success)
    painchance = 0; // default
  lua_pop(L, 1);

  // speed
  lua_getfield(L, -1, "speed");
  speed = lua_tointegerx(L, -1, &success);
  if (!success)
    speed = 0; // default
  lua_pop(L, 1);

  // radius
  lua_getfield(L, -1, "radius");
  radius = lua_tonumberx(L, -1, &success);
  if (!success)
    radius = 20.0; // default
  lua_pop(L, 1);

  // height
  lua_getfield(L, -1, "height");
  height = lua_tonumberx(L, -1, &success);
  if (!success)
    height = 16.0; // default
  lua_pop(L, 1);

  // mass
  lua_getfield(L, -1, "mass");
  mass = lua_tointegerx(L, -1, &success);
  if (!success)
    mass = 100; // default
  lua_pop(L, 1);

  // damage
  lua_getfield(L, -1, "damage");
  damage = lua_tointegerx(L, -1, &success);
  if (!success)
    damage = 0; // default
  lua_pop(L, 1);

  mobj = lua_newuserdata(L, sizeof(mobjinfo_t));
  mobj->doomednum = doomednum;
  mobj->spawnhealth = spawnhealth;
  mobj->reactiontime = reactiontime;
  mobj->painchance = painchance;
  mobj->speed = speed;
  mobj->radius = (int)(radius * (double)FRACUNIT);
  mobj->height = (int)(height * (double)FRACUNIT);
  mobj->mass = mass;
  mobj->damage = damage;

  luaL_getmetatable(L, "Doom.mobj");
  lua_setmetatable(L, -2);

  return 1;
}

static const luaL_Reg mobjlib_functions[] =
{
  {"new", mobjlib_new},
  {NULL, NULL},
};

/**
 * Load the mobj library
 */
int luaopen_mobj(lua_State* L)
{
  // Userdata metatables
  luaL_newmetatable(L, "Doom.mobj");

  // Library methods
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");
  luaL_setfuncs(L, mobjlib_methods, 0);

  // Library functions
  luaL_newlib(L, mobjlib_functions);
  return 1;
}
