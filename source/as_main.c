// Emacs style mode select   -*- C++ -*-
//-----------------------------------------------------------------------------
//
//  Copyright (C) 1999 by
//  id Software, Chi Hoang, Lee Killough, Jim Flynn, Rand Phares, Ty Halderman,
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
//   AngelScript initialization.
//
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stddef.h>

#include "angelscript_c.h"

asIScriptEngine* as_engine = NULL;
asIScriptContext* as_ctx = NULL;

void AS_Init()
{
  printf("AS_Init: ");

  as_engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
  as_ctx = asEngine_CreateContext(as_engine);

  printf("Init AngelScript " ANGELSCRIPT_VERSION_STRING ".\n");
}
