/*
fset_update

  Takes a function_set (from app_build_function_sets) and
  removes all functions that are not specified thusly:

    app.use_FunctionRepString=1

  in the input file.

  NOTE:  if no specification exists then enabled is assumed

  so 

    {and,		NULL, NULL, 2, "AND",	FUNC_EXPR, -1, 0, BOOL, {BOOL,BOOL}}

  would be disabled by adding

    app.use_AND=0

  to the input file.

  This is very handy when working through to see if a 
  function or terminal is useful.

  NOTE:  function representitive strings should be unique, both
         because of the matching done here and because lilgp assumes 
         they are unique in various places.

  Written by: Adam Hewgill (C) 2002
*/
#include <lilgp.h>
#include <stdlib.h>


void fset_update (function_set *app_fset)
{
  char name[80], *param;
  int i, j;
  int numrem = 0;
  function *fset = app_fset->cset;
  int size = app_fset->size;

  for (i = 0; i < size - numrem; ++i)
  {
    /* Setup name string */
    sprintf(name, "app.use_%s", fset[i].string);

    /* Get value of parameter in input file */
    param = get_parameter(name);

    /* If not specified assume enabled */
    if (param == NULL)
      continue;

    /* Remove if value of param is <= 0 */
    if (atoi(param) <= 0)
    {
      /* Output function removed */
      oprintf ( OUT_SYS, 30, " %s", fset[i].string); 
      /* Roll rest up */
      for (j = i + 1; j < size - numrem; ++j)
        fset[j - 1] = fset[j];
      ++numrem;
      --i;
    }
  }
  /* Update the set size */
  app_fset->size = size - numrem;
  /* Print final message */
  oprintf ( OUT_SYS, 30, " removed.\n"); 
}
