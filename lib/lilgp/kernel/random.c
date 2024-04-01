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
#include <pthread.h>

/*
 * adapted from the RAN3 routine (in Fortran - ugh) in "Numerical Recipes:
 * The Art of Scientific Computing", page 199.  They adapted it from
 * (surprise) Knuth, Seminumerical Algorithms.
 */


/* random_seed()
 *
 * seeds the random number generator using the given int.
 */

void random_seed ( randomgen *randstr, int seed )
{
     int i, i1, k;
     double mj, mk, ms;
     
     pthread_mutex_init(&randstr->rmut, NULL);

     randstr->mbig = 10000000.0;
     randstr->mseed = 1618033.0;
     randstr->mz = 0.0;
     
     mj = randstr->mseed - seed;
     ms = (mj<0)? -0.5 : 0.5;
     mj = mj-((int)((mj/randstr->mbig)+ms)*randstr->mbig);
     randstr->ma[54] = mj;
     mk = 1;
     
     for ( i = 1; i < 55; ++i )
     {
          i1 = ((21*i) % 55)-1;
          randstr->ma[i1] = mk;
          mk = mj - mk;
          if ( mk < randstr->mz )
               mk += randstr->mbig;
          mj = randstr->ma[i1];
     }
     
     for ( k = 0; k < 4; ++k )
          for ( i = 0; i < 55; ++i )
          {
               randstr->ma[i] = randstr->ma[i] - randstr->ma[(i+30)%55];
               if ( randstr->ma[i] < randstr->mz )
                    randstr->ma[i] += randstr->mbig;
          }
     
     randstr->inext = 0;
     randstr->inextp = 31;     /* the number 31 is special -- see Knuth. */
     
}


/* random_destroy()
 *
 * destroys the mutex with the random structure
 */

void random_destroy ( randomgen *randstr )
{
     pthread_mutex_destroy(&randstr->rmut);
}

     
/* random_int()
 *
 * returns an integer randomly selected from the uniform distribution
 * over the interval [0,max).
 */

int random_int ( randomgen *randstr, int max )
{
     double v = random_double(randstr);
     return (int)(v*(double)max);
}

/* random_double()
 *
 * returns a double randomly selected from the uniform distribution
 * over the interval [0,1).
 */

double random_double ( randomgen *randstr )
{
  /* There's a race condition on this, so we need to mutex it! */

     double mj;
     double res;

     pthread_mutex_lock(&(randstr->rmut));

     randstr->inext = (randstr->inext+1)%55;
     randstr->inextp = (randstr->inextp+1)%55;
     
     mj = randstr->ma[randstr->inext] - randstr->ma[randstr->inextp];
     if ( mj < randstr->mz )
          mj = mj + randstr->mbig;
     randstr->ma[randstr->inext] = mj;

     res=randstr->mbig;
     pthread_mutex_unlock(&(randstr->rmut));
     return mj/res;
}

/* random_get_state()
 *
 * allocates a memory block, saves the state of the random number
 * generator in it, and returns the address.  puts the number of
 * bytes in the block into *size.
 */

void *random_get_state ( randomgen *randstr, int *size )
{
     unsigned char *buffer;
     double *db;
     int i;

     *size = sizeof(double)*58+2*sizeof(int);
     
     buffer = (unsigned char *)MALLOC ( *size );
     db = (double *)buffer;

     db[0] = randstr->mbig;
     db[1] = randstr->mseed;
     db[2] = randstr->mz;
     for ( i = 0; i < 55; ++i )
          db[i+3] = randstr->ma[i];
     ((int *)(buffer+58*sizeof(double)))[0] = randstr->inext;
     ((int *)(buffer+58*sizeof(double)))[1] = randstr->inextp;

#ifdef DEBUG
     fprintf ( stderr, "writing random state: %lf, %lf, %lf, %d, %d\n",
             randstr->mbig, randstr->mseed, randstr->mz, randstr->inext, randstr->inextp );
#endif

     return buffer;
     
}

/* random_set_state()
 *
 * restores the random number generator state using a block of
 * data previously returned by random_get_state().
 */

void random_set_state ( randomgen *randstr, void *buffer )
{
     unsigned char *cb;
     double *db;
     int i;

     cb = (unsigned char *)buffer;
     db = (double *)buffer;

     randstr->mbig = db[0];
     randstr->mseed = db[1];
     randstr->mz = db[2];
     for ( i = 0; i < 55; ++i )
          randstr->ma[i] = db[i+3];
     randstr->inext = ((int *)(cb+58*sizeof(double)))[0];
     randstr->inextp = ((int *)(cb+58*sizeof(double)))[1];

#ifdef DEBUG
     fprintf ( stderr, "reading random state: %lf, %lf, %lf, %d, %d\n",
             randstr->mbig, randstr->mseed, randstr->mz, randstr->inext, randstr->inextp );
#endif
     
}

