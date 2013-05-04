// Emacs style mode select   -*- C++ -*-
//-----------------------------------------------------------------------------
//
//  Copyright (C) 1999 by
//  id Software, Chi Hoang, Lee Killough, Jim Flynn, Rand Phares, Ty Halderman
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
//      Cheat code checking.
//
//-----------------------------------------------------------------------------

#ifndef __M_CHEAT__
#define __M_CHEAT__

// killough 4/16/98: Cheat table structure

extern struct cheat_s
{
  const unsigned char* cheat;
  const char* const deh_cheat;
  enum
  {
    always   = 0,
    not_dm   = 1,
    not_coop = 2,
    not_demo = 4,
    not_menu = 8,
    not_deh  = 16,
    beta_only = 32,                  // killough 7/24/98
    not_net = not_dm | not_coop
  } const when;
  void (*const func)();
  const int arg;
  ULong64 code, mask;
  boolean deh_modified;                // killough 9/12/98
} cheat[];

boolean M_FindCheats(int key);

extern int idmusnum;

#endif
