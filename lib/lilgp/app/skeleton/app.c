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

#include <lilgp.h>

/* Possible Parameter/Return Types
#define NONE	-1
#define BOOL	0
#define FLOAT	1
*/

int app_build_function_sets ( void )
{
     function_set fset;
     user_treeinfo tree_map;
     /* -- make sure "name" is unique over set --
     function sets[] =
     {{f_func_expr, NULL, NULL, 2, "func_expr", FUNC_EXPR, -1, 0, 
         BOOL, {BOOL, BOOL, NONE, NONE}}
     ,{f_func_data, NULL, NULL, 2, "func_data", FUNC_DATA, -1, 0, 
         FLOAT, {FLOAT, BOOL, NONE, NONE}}
     ,{f_terminal,  NULL, NULL, 0, "terminal",  TERM_NORM, -1, 0, 
         BOOL, {NONE, NONE, NONE, NONE}}
     };

     // Setup function set structure
     fset.size = 3;
     fset.cset = sets;
     
     // Setup tree map with function set
     tree_map.fset = 0;
     tree_map.return_type = BOOL;
     tree_map.name = "My Tree";

     */
     return function_sets_init ( &fset, 1, &tree_map, 1 );
}


int app_initialize ( int startfromcheckpoint )
{
     // Get the a ptr to the global data structure
     globaldata* g = get_globaldata();
     
     // Initialize the local random number generator
     random_seed(&g->myrand, 0);

     return 0;
}


void app_uninitialize ( void )
{
     // Destroy the random number structure
     random_destroy(&g->myrand);
}


void app_eval_fitness ( individual *ind )
{
     
}


int app_end_of_evaluation ( int gen, multipop *mpop, int newbest,
                           popstats *gen_stats, popstats *run_stats )
{
     return 0;
}


void app_end_of_breeding ( int gen, multipop *mpop )
{
     return;
}


void app_write_checkpoint ( FILE *f )
{
     int random_state_bytes;
     unsigned char *rand_state;
     // Get the a ptr to the global data structure
     globaldata* g = get_globaldata();
     
     // write the state of the random number generator
     rand_state = random_get_state_str ( &g->myrand, &random_state_bytes );
     fprintf ( f, "random-state: %d ", random_state_bytes );
     // store buffer as hex data
     write_hex_block ( rand_state, random_state_bytes, f );
     fputc ( '\n', f );
     FREE ( rand_state );
}


void app_read_checkpoint ( FILE *f )
{
     int random_state_bytes;
     unsigned char *rand_state;
     // Get the a ptr to the global data structure
     globaldata* g = get_globaldata();
     
     // Read the size of the state
     fscanf ( f, "%*s %d ", &random_state_bytes );
     // allocate the buffer
     rand_state = (char *)MALLOC ( random_state_bytes+1 );
     // read the hex data into the buffer. */
     read_hex_block ( rand_state, random_state_bytes, f );
     // set the state
     random_set_state_str ( &g->myrand, rand_state );
     FREE ( rand_state );
     fgetc ( f );
}


int app_create_output_streams ( void )
{
     return 0;
}



