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

#ifndef _APPDEF_H
#define _APPDEF_H

#define MAXARGS    4
#define DATATYPE   int

/* Define NUMTYPES to be the number of types passed and returned in your
   GP program.  You should distinguish your types from 0 to NUMTYPES-1 */
#define NUMTYPES 2   /* Void (what move etc. do) and bool */


#define APP_EMPTY     0
#define APP_FOOD      1
#define APP_PATH      2
#define APP_ANT       3

#define APP_EAST      0
#define APP_NORTH     1
#define APP_WEST      2
#define APP_SOUTH     3

#endif
