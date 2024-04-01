/*  lil-gp Genetic Programming System, version 1.0, 11 July 1995
 *  Copyright (C) 1995  Michigan State University
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of version 2 of the GNU General Public License as
 *  published by the Free Software Foundation.
 * 
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *  
 *  Douglas Zongker       (zongker@isl.cps.msu.edu)
 *  Dr. Bill Punch        (punch@isl.cps.msu.edu)
 *
 *  Computer Science Department
 *  A-714 Wells Hall
 *  Michigan State University
 *  East Lansing, Michigan  48824
 *  USA
 *  
 */

#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include <lilgp.h>

DATATYPE f_multiply ( int tree, farg *args );
DATATYPE f_protdivide ( int tree, farg *args );
DATATYPE f_add ( int tree, farg *args );
DATATYPE f_subtract ( int tree, farg *args );
DATATYPE f_sin ( int tree, farg *args );
DATATYPE f_cos ( int tree, farg *args );
DATATYPE f_exp ( int tree, farg *args );
DATATYPE f_rlog ( int tree, farg *args );
DATATYPE f_indepvar ( int tree, farg *args );

void f_erc_gen ( DATATYPE * );
char *f_erc_print ( DATATYPE );

#endif
