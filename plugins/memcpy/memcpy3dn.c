/*****************************************************************************
 * memcpy3dn.c : 3D Now! memcpy module
 *****************************************************************************
 * Copyright (C) 2001 VideoLAN
 * $Id: memcpy3dn.c,v 1.3 2001/12/30 07:09:55 sam Exp $
 *
 * Authors: Samuel Hocevar <sam@zoy.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111, USA.
 *****************************************************************************/

/*****************************************************************************
 * Preamble
 *****************************************************************************/
#include <stdlib.h>
#include <string.h>

#include <videolan/vlc.h>

/*****************************************************************************
 * Local and extern prototypes.
 *****************************************************************************/
static void memcpy_getfunctions( function_list_t * p_function_list );
static int  memcpy_Probe       ( probedata_t *p_data );
void *      _M( fast_memcpy )  ( void * to, const void * from, size_t len );

#undef HAVE_MMX
#undef HAVE_MMX2
#undef HAVE_SSE
#undef HAVE_SSE2
#define HAVE_3DNOW
#include "fastmemcpy.h"

/*****************************************************************************
 * Build configuration tree.
 *****************************************************************************/
MODULE_CONFIG_START
MODULE_CONFIG_STOP

MODULE_INIT_START
    SET_DESCRIPTION( "3D Now! memcpy module" )
    ADD_CAPABILITY( MEMCPY, 100 )
    ADD_REQUIREMENT( 3DNOW )
    ADD_SHORTCUT( "3dnow" )
    ADD_SHORTCUT( "3dn" )
MODULE_INIT_STOP

MODULE_ACTIVATE_START
    memcpy_getfunctions( &p_module->p_functions->memcpy );
MODULE_ACTIVATE_STOP

MODULE_DEACTIVATE_START
MODULE_DEACTIVATE_STOP

/* Following functions are local */

/*****************************************************************************
 * Functions exported as capabilities. They are declared as static so that
 * we don't pollute the namespace too much.
 *****************************************************************************/
static void memcpy_getfunctions( function_list_t * p_function_list )
{
    p_function_list->pf_probe = memcpy_Probe;
#define F p_function_list->functions.memcpy
    F.fast_memcpy = _M( fast_memcpy );
#undef F
}

/*****************************************************************************
 * memcpy_Probe: returns a preference score
 *****************************************************************************/
static int memcpy_Probe( probedata_t *p_data )
{
    /* This plugin always works */
    return( 100 );
}

