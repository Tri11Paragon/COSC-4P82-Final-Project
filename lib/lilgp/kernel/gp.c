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

popstats* run_stats;
saved_ind* saved_head, * saved_tail;

#if !defined(POSIX_MT) && !defined(SOLARIS_MT)

globaldata global_g;

#else

int numthreads = 0;

struct thread_param_t
{
    population* pop;
    int startidx;
    int endidx;
    globaldata g;
};

#ifdef POSIX_MT

#include <pthread.h>

static pthread_key_t g_key;
static pthread_attr_t pthread_attr;
#endif

#ifdef SOLARIS_MT
#include <thread.h>
static thread_key_t         g_key;
#endif

#endif /* !defined(POSIX_MT) && !defined(SOLARIS_MT) */

/* run_gp()
 *
 * the whole enchilada.  runs, from generation startgen, using population
 * mpop.  accumulates time spent evaluating and breeding in t_eval and t_breed.
 */

void run_gp(multipop* mpop, int startgen,
            event* t_eval, event* t_breed, int startfromcheckpoint)
{
    
    char* param;
    int gen;
    int maxgen;
    int exch_gen;
    int i, j;
    int checkinterval;
    char* checkfileformat;
    char* checkfilename = NULL;
    event start, end, diff;
    int term = 0;
    int stt_interval;
    int bestn;
    
    if (!startfromcheckpoint)
    {
        
        /* get the number of top individuals to track. */
        bestn = atoi(get_parameter("output.bestn"));
        if (bestn < 1)
        {
            error(E_WARNING, "\"output.bestn\" must be at least 1.  defaulting to 1.");
            bestn = 1;
        }
        
        /* allocate statistics for overall run. */
        run_stats = (popstats*) MALLOC((mpop->size + 1) * sizeof(popstats));
        for (i = 0; i < mpop->size + 1; ++i)
        {
            run_stats[i].bestn = bestn;
            run_stats[i].size = -1;
        }
        
        /* initialize the linked list of saved individuals. */
        saved_head = (saved_ind*) MALLOC(sizeof(saved_ind));
        saved_head->ind = NULL;
        saved_head->refcount = 0;
        saved_head->next = NULL;
        saved_tail = saved_head;
    }
    
    /* get the maximum number of generations. */
    param = get_parameter("max_generations");
    if (param == NULL)
        error(E_FATAL_ERROR,
              "no value specified for \"max_generations\".");
    maxgen = atoi(param);
    if (maxgen <= 0)
        error(E_FATAL_ERROR,
              "\"max_generations\" must be greater than zero.");
    
    /* get the interval for subpopulation exchanges, if there is more than
       one subpopulation. */
    if (mpop->size > 1)
    {
        param = get_parameter("multiple.exch_gen");
        if (param == NULL)
            error(E_FATAL_ERROR,
                  "no value specified for \"multiple.exch_gen\".");
        exch_gen = atoi(param);
        if (exch_gen <= 0)
            error(E_FATAL_ERROR,
                  "\"multiple.exch_gen\" must be greater than zero.");
    }
    
    /* get the interval for doing checkpointing. */
    param = get_parameter("checkpoint.interval");
    if (param == NULL)
        /* checkpointing disabled. */
        checkinterval = -1;
    else
        checkinterval = atoi(param);
    
    /* get the format string for the checkpoint filenames. */
    checkfileformat = get_parameter("checkpoint.filename");
    checkfilename = (char*) MALLOC(strlen(checkfileformat) + 50);
    
    /* get the interval for writing information to the .stt file. */
    stt_interval = atoi(get_parameter("output.stt_interval"));
    if (stt_interval < 1)
        error(E_FATAL_ERROR,
              "\"output.stt_interval\" must be greater than zero.");
    
    oputs(OUT_SYS, 10, "\n\nstarting evolution.\n");
    
    /* print out how often we'll be doing checkpointing. */
    if (checkinterval > 0)
        oprintf(OUT_SYS, 20,
                "checkpointing will be done every %d generations and "\
          "after the last generation.\n", checkinterval);
    else if (checkinterval == 0)
        oprintf(OUT_SYS, 20,
                "checkpointing will be done only after the last "\
          "generation.\n");
    else
        oprintf(OUT_SYS, 20,
                "no checkpointing will be done.\n");
    
    output_stream_open(OUT_STT);
    oprintf(OUT_STT, 50, "GEN#\tSUB#\tμFGEN\tFsBestGEN\tFsWorstGEN\tμTreeSzGEN\tμTreeDpGEN\tbTreeSzGEN\tbTreeDpGEN\twTreeSzGEN\twTreeDpGEN\tμFRUN\t"
                         "FsBestRUN\tFsWorstRUN\tμTreeSzRUN\tμTreeDpRUN\tbTreeSzRUN\tbTreeDpRUN\twTreeSzRUN\twTreeDpRUN\n");
    
    /* the big loop. */
    for (gen = startgen; gen <= maxgen && !term; ++gen)
    {
        oprintf(OUT_SYS, 20,
                "=== generation %d.\n", gen);
        app_begin_of_evaluation(gen, mpop);
        
        /* unless this is the first generation after loading a checkpoint
       file... */
        if (!(startfromcheckpoint && gen == startgen))
        {
            
            /* evaluate the population. */
            event_mark(&start);
            for (i = 0; i < mpop->size; ++i)
                evaluate_pop(mpop->pop[i]);
            event_mark(&end);
            event_diff(&diff, &start, &end);

#ifdef TIMING_AVAILABLE
            oprintf(OUT_SYS, 40, "    evaluation complete.  (%s)\n",
                    event_string(&diff));
#else
            oprintf ( OUT_SYS, 40, "    evaluation complete.\n" );
#endif
            
            event_accum(t_eval, &diff);
            
            /* calculate and print statistics.  returns 1 if user termination
               criterion was met, 0 otherwise. */
            term = generation_information(gen, mpop, stt_interval,
                                          run_stats[0].bestn);
            if (term)
                oprintf(OUT_SYS, 30, "user termination criterion met.\n");
            
            flush_output_streams();
            
        }
        
        /** write a checkpoint file if checkinterval is non-negative and:
      we've reached the last generation, or
      the user termination criterion has been met, or
      we've reached the specified checkpoint interval. **/
        if (checkinterval >= 0 &&
            (gen == maxgen || term ||
             (checkinterval > 0 && gen > startgen && (gen % checkinterval) == 0)))
        {
            sprintf(checkfilename, checkfileformat, gen);
            write_checkpoint(gen, mpop, checkfilename);
        }
        
        /** if this is not the last generation and the user criterion hasn't
      been met, then do breeding. **/
        if (gen != maxgen && !term)
        {
            
            /** exchange subpops if it's time. **/
            if (mpop->size > 1 && gen && (gen % exch_gen) == 0)
            {
                exchange_subpopulations(mpop);
                oprintf(OUT_SYS, 10,
                        "    subpopulation exchange complete.\n");
            }
            
            /* breed the new population. */
            event_mark(&start);
            for (i = 0; i < mpop->size; ++i)
                mpop->pop[i] = change_population(mpop->pop[i], mpop->bpt[i]);
            event_mark(&end);
            event_diff(&diff, &start, &end);
            
            /* call the application end-of-breeding callback. */
            app_end_of_breeding(gen, mpop);

#ifdef TIMING_AVAILABLE
            oprintf(OUT_SYS, 30, "    breeding complete.    (%s)\n",
                    event_string(&diff));
#else
            oprintf ( OUT_SYS, 30, "    breeding complete.\n" );
#endif
            
            event_accum(t_breed, &diff);
            
        }
        
        /* free unused ERCs. */
        ephem_const_gc();
        
        flush_output_streams();
        
    }
    
    /** free up a lot of stuff before returning. */
    
    if (checkfilename)
        FREE(checkfilename);
    
    ephem_const_gc();
    
    for (i = 0; i < mpop->size + 1; ++i)
    {
        for (j = 0; j < run_stats[i].bestn; ++j)
            --run_stats[i].best[j]->refcount;
        FREE(run_stats[i].best);
    }
    FREE(run_stats);
    
    saved_individual_gc();
    FREE(saved_head);
}

/* generation_information()
 *
 * calculates and prints population statistics.
 */

int generation_information(int gen, multipop* mpop, int stt_interval,
                           int bestn)
{
    int i, j;
    int newbest;
    static int fd = -1;
    popstats* gen_stats;
    int ret = 0;
    FILE* bout, * hout;
    
    /* number of decimal digits to use when printing fitness values. */
    if (fd == -1)
        fd = atoi(get_parameter("output.digits"));
    
    /* allocate stats records for the current generation. */
    gen_stats = (popstats*) MALLOC((mpop->size + 1) * sizeof(popstats));
    for (i = 0; i < mpop->size + 1; ++i)
    {
        gen_stats[i].bestn = bestn;
        gen_stats[i].size = -1;
    }
    
    oprintf(OUT_GEN, 90, "=== GENERATION %d ===\n", gen);
    oprintf(OUT_PRG, 90, "=== GENERATION %d ===\n", gen);
    
    /* for each subpopulation... */
    for (i = 0; i < mpop->size; ++i)
    {
        /* calculate stats for subpopulation. */
        calculate_pop_stats(gen_stats + i + 1, mpop->pop[i], gen, i);
        /* accumulate that into stats for whole popluation... */
        accumulate_pop_stats(gen_stats, gen_stats + i + 1);
        /* ...and stats for this subpopulation over the whole run. */
        accumulate_pop_stats(run_stats + i + 1, gen_stats + i + 1);
        
        /* if only one subpop, don't print out the subpop stuff. */
        if (mpop->size == 1)
            continue;
        
        /** print much stuff to .gen, .prg, and .stt files. */
        
        if (test_detail_level(90))
        {
            oprintf(OUT_GEN, 90, "    subpopulation %d:\n", i + 1);
            oprintf(OUT_GEN, 90, "        generation:\n");
            oprintf(OUT_GEN, 90, "            mean:   nodes: %.3lf (%d-%d); depth: %.3lf (%d-%d)\n",
                    (double) gen_stats[i + 1].totalnodes / gen_stats[i + 1].size,
                    gen_stats[i + 1].minnodes, gen_stats[i + 1].maxnodes,
                    (double) gen_stats[i + 1].totaldepth / gen_stats[i + 1].size,
                    gen_stats[i + 1].mindepth, gen_stats[i + 1].maxdepth);
            oprintf(OUT_GEN, 90, "            best:   nodes: %d; depth: %d\n",
                    gen_stats[i + 1].bestnodes, gen_stats[i + 1].bestdepth);
            oprintf(OUT_GEN, 90, "            worst:  nodes: %d; depth: %d\n",
                    gen_stats[i + 1].worstnodes, gen_stats[i + 1].worstdepth);
            oprintf(OUT_GEN, 90, "        run:   (%d trees)\n",
                    run_stats[i + 1].size);
            oprintf(OUT_GEN, 90, "            mean:   nodes: %.3lf (%d-%d); depth: %.3lf (%d-%d)\n",
                    (double) run_stats[i + 1].totalnodes / run_stats[i + 1].size,
                    run_stats[i + 1].minnodes, run_stats[i + 1].maxnodes,
                    (double) run_stats[i + 1].totaldepth / run_stats[i + 1].size,
                    run_stats[i + 1].mindepth, run_stats[i + 1].maxdepth);
            oprintf(OUT_GEN, 90, "            best:   nodes: %d; depth: %d\n",
                    run_stats[i + 1].bestnodes, run_stats[i + 1].bestdepth);
            oprintf(OUT_GEN, 90, "            worst:  nodes: %d; depth: %d\n",
                    run_stats[i + 1].worstnodes, run_stats[i + 1].worstdepth);
        }
        
        if (test_detail_level(90))
        {
            oprintf(OUT_PRG, 90, "    subpopulation %d:\n", i + 1);
            oprintf(OUT_PRG, 90, "        generation stats:\n");
            oprintf(OUT_PRG, 90, "            mean:   hits: %.3lf (%d-%d); standardized fitness: %.*lf\n",
                    (double) gen_stats[i + 1].totalhits / gen_stats[i + 1].size,
                    gen_stats[i + 1].minhits, gen_stats[i + 1].maxhits,
                    fd, (double) gen_stats[i + 1].totalfit / gen_stats[i + 1].size);
            oprintf(OUT_PRG, 90, "            best:   hits: %d; standardized fitness: %.*lf\n",
                    gen_stats[i + 1].besthits, fd,
                    (double) gen_stats[i + 1].bestfit);
            oprintf(OUT_PRG, 90, "            worst:  hits: %d; standardized fitness: %.*lf\n",
                    gen_stats[i + 1].worsthits, fd,
                    (double) gen_stats[i + 1].worstfit);
            oprintf(OUT_PRG, 90, "        run stats:     (%d trees)\n",
                    run_stats[i + 1].size);
            oprintf(OUT_PRG, 90, "            mean:   hits: %.3lf (%d-%d); standardized fitness: %.*lf\n",
                    (double) run_stats[i + 1].totalhits / run_stats[i + 1].size,
                    run_stats[i + 1].minhits, run_stats[i + 1].maxhits,
                    fd, (double) run_stats[i + 1].totalfit / run_stats[i + 1].size);
            oprintf(OUT_PRG, 90, "            best:   hits: %d; standardized fitness: %.*lf; generation: %d\n",
                    run_stats[i + 1].besthits, fd, (double) run_stats[i + 1].bestfit,
                    run_stats[i + 1].bestgen);
            oprintf(OUT_PRG, 90, "            worst:  hits: %d; standardized fitness: %.*lf; generation: %d\n",
                    run_stats[i + 1].worsthits, fd,
                    (double) run_stats[i + 1].worstfit, run_stats[i + 1].worstgen);
        }
        
        if (gen % stt_interval == 0)
        {
            oprintf(OUT_STT, 50, "%d\t%d\t", gen, i + 1);
            oprintf(OUT_STT, 50, "%.*lf\t%.*lf %.*lf\t",
                    fd, gen_stats[i + 1].totalfit / gen_stats[i + 1].size,
                    fd, gen_stats[i + 1].bestfit,
                    fd, gen_stats[i + 1].worstfit);
            oprintf(OUT_STT, 50, "%.3lf\t%.3lf\t%d\t%d\t%d\t%d\t",
                    (double) gen_stats[i + 1].totalnodes / gen_stats[i + 1].size,
                    (double) gen_stats[i + 1].totaldepth / gen_stats[i + 1].size,
                    gen_stats[i + 1].bestnodes, gen_stats[i + 1].bestdepth,
                    gen_stats[i + 1].worstnodes, gen_stats[i + 1].worstdepth);
            oprintf(OUT_STT, 50, "%.*lf\t%.*lf\t%.*lf\t",
                    fd, run_stats[i + 1].totalfit / run_stats[i + 1].size,
                    fd, run_stats[i + 1].bestfit,
                    fd, run_stats[i + 1].worstfit);
            oprintf(OUT_STT, 50, "%.3lf\t%.3lf\t%d\t%d\t%d\t%d\t",
                    (double) run_stats[i + 1].totalnodes / run_stats[i + 1].size,
                    (double) run_stats[i + 1].totaldepth / run_stats[i + 1].size,
                    run_stats[i + 1].bestnodes, run_stats[i + 1].bestdepth,
                    run_stats[i + 1].worstnodes, run_stats[i + 1].worstdepth);
            oprintf(OUT_STT, 50, "\n");
        }
        
    }
    
    /* merge stats for current generation into overall run stats. */
    newbest = accumulate_pop_stats(run_stats, gen_stats);
    
    /** more printing. **/
    
    if (test_detail_level(90))
    {
        oprintf(OUT_GEN, 90, "    total population:\n");
        oprintf(OUT_GEN, 90, "        generation:\n");
        oprintf(OUT_GEN, 90, "            mean:   nodes: %.3lf (%d-%d); depth: %.3lf (%d-%d)\n",
                (double) gen_stats[0].totalnodes / gen_stats[0].size,
                gen_stats[0].minnodes, gen_stats[0].maxnodes,
                (double) gen_stats[0].totaldepth / gen_stats[0].size,
                gen_stats[0].mindepth, gen_stats[0].maxdepth);
        oprintf(OUT_GEN, 90, "            best:   nodes: %d; depth: %d\n",
                gen_stats[0].bestnodes, gen_stats[0].bestdepth);
        oprintf(OUT_GEN, 90, "            worst:  nodes: %d; depth: %d\n",
                gen_stats[0].worstnodes, gen_stats[0].worstdepth);
        oprintf(OUT_GEN, 90, "        run:    (%d trees)\n",
                run_stats[0].size);
        oprintf(OUT_GEN, 90, "            mean:   nodes: %.3lf (%d-%d); depth: %.3lf (%d-%d)\n",
                (double) run_stats[0].totalnodes / run_stats[0].size,
                run_stats[0].minnodes, run_stats[0].maxnodes,
                (double) run_stats[0].totaldepth / run_stats[0].size,
                run_stats[0].mindepth, run_stats[0].maxdepth);
        oprintf(OUT_GEN, 90, "            best:   nodes: %d; depth: %d\n",
                run_stats[0].bestnodes, run_stats[0].bestdepth);
        oprintf(OUT_GEN, 90, "            worst:  nodes: %d; depth: %d\n",
                run_stats[0].worstnodes, run_stats[0].worstdepth);
    }
    
    if (test_detail_level(90))
    {
        oprintf(OUT_PRG, 90, "    total population:\n");
        oprintf(OUT_PRG, 90, "        generation stats:\n");
        oprintf(OUT_PRG, 90, "            mean:   hits: %.3lf (%d-%d); standardized fitness: %.*lf\n",
                (double) gen_stats[0].totalhits / gen_stats[0].size,
                gen_stats[0].minhits, gen_stats[0].maxhits,
                fd, (double) gen_stats[0].totalfit / gen_stats[0].size);
        oprintf(OUT_PRG, 90, "            best:   hits: %d; standardized fitness: %.*lf\n",
                gen_stats[0].besthits, fd, (double) gen_stats[0].bestfit);
        oprintf(OUT_PRG, 90, "            worst:  hits: %d; standardized fitness: %.*lf\n",
                gen_stats[0].worsthits, fd, (double) gen_stats[0].worstfit);
        oprintf(OUT_PRG, 90, "        run stats:     (%d trees)\n",
                run_stats[0].size);
        oprintf(OUT_PRG, 90, "            mean:   hits: %.3lf (%d-%d); standardized fitness: %.*lf\n",
                (double) run_stats[0].totalhits / run_stats[0].size,
                run_stats[0].minhits, run_stats[0].maxhits,
                fd, (double) run_stats[0].totalfit / run_stats[0].size);
        oprintf(OUT_PRG, 90, "            best:   hits: %d; standardized fitness: %.*lf; generation: %d\n",
                run_stats[0].besthits, fd, (double) run_stats[0].bestfit,
                run_stats[0].bestgen);
        oprintf(OUT_PRG, 90, "            worst:  hits: %d; standardized fitness: %.*lf; generation: %d\n",
                run_stats[0].worsthits, fd, (double) run_stats[0].worstfit,
                run_stats[0].worstgen);
    }
    
    if (gen % stt_interval == 0)
    {
        if (test_detail_level(50))
        {
            oprintf(OUT_STT, 50, "%d\t0\t", gen);
            oprintf(OUT_STT, 50, "%.*lf\t%.*lf\t%.*lf\t",
                    fd, gen_stats[0].totalfit / gen_stats[0].size,
                    fd, gen_stats[0].bestfit, fd, gen_stats[0].worstfit);
            oprintf(OUT_STT, 50, "%.3lf\t%.3lf\t%d\t%d\t%d\t%d\t",
                    (double) gen_stats[0].totalnodes / gen_stats[0].size,
                    (double) gen_stats[0].totaldepth / gen_stats[0].size,
                    gen_stats[0].bestnodes, gen_stats[0].bestdepth,
                    gen_stats[0].worstnodes, gen_stats[0].worstdepth);
            oprintf(OUT_STT, 50, "%.*lf\t%.*lf\t%.*lf\t",
                    fd, run_stats[0].totalfit / run_stats[0].size,
                    fd, run_stats[0].bestfit, fd, run_stats[0].worstfit);
            oprintf(OUT_STT, 50, "%.3lf\t%.3lf\t%d\t%d\t%d\t%d\t",
                    (double) run_stats[0].totalnodes / run_stats[0].size,
                    (double) run_stats[0].totaldepth / run_stats[0].size,
                    run_stats[0].bestnodes, run_stats[0].bestdepth,
                    run_stats[0].worstnodes, run_stats[0].worstdepth);
            oprintf(OUT_STT, 50, "\n");
        }
    }
    
    /* rewrite the .bst file, and append to the .his file. */
    
    output_stream_open(OUT_BST);
    
    oprintf(OUT_BST, 10, "=== BEST-OF-RUN ===\n");
    oprintf(OUT_BST, 10, "              generation: %d\n",
            run_stats[0].bestgen);
    if (mpop->size > 1)
        oprintf(OUT_BST, 10, "           subpopulation: %d\n",
                run_stats[0].bestpop + 1);
    oprintf(OUT_BST, 10, "                   nodes: %d\n",
            run_stats[0].bestnodes);
    oprintf(OUT_BST, 10, "                   depth: %d\n",
            run_stats[0].bestdepth);
    oprintf(OUT_BST, 10, "                    hits: %d\n",
            run_stats[0].besthits);
    
    oprintf(OUT_HIS, 10, "=== BEST-OF-RUN ===\n");
    oprintf(OUT_HIS, 10, "      current generation: %d\n", gen);
    oprintf(OUT_HIS, 10, "              generation: %d\n",
            run_stats[0].bestgen);
    if (mpop->size > 1)
        oprintf(OUT_HIS, 10, "           subpopulation: %d\n",
                run_stats[0].bestpop + 1);
    oprintf(OUT_HIS, 10, "                   nodes: %d\n",
            run_stats[0].bestnodes);
    oprintf(OUT_HIS, 10, "                   depth: %d\n",
            run_stats[0].bestdepth);
    oprintf(OUT_HIS, 10, "                    hits: %d\n",
            run_stats[0].besthits);
    
    /* retrieve the (FILE *) for the .bst and .his files, so that
       the trees can be printed to them. */
    
    bout = output_filehandle(OUT_BST);
    hout = output_filehandle(OUT_HIS);
    
    if (run_stats[0].bestn == 1)
    {
        oprintf(OUT_BST, 20, "TOP INDIVIDUAL:\n\n");
        oprintf(OUT_HIS, 20, "TOP INDIVIDUAL:\n\n");
    } else
    {
        oprintf(OUT_BST, 20, "TOP %d INDIVIDUALS (in order):\n\n",
                run_stats[0].bestn);
        oprintf(OUT_HIS, 20, "TOP %d INDIVIDUALS (in order):\n\n",
                run_stats[0].bestn);
    }
    
    for (i = 0; i < run_stats[0].bestn; ++i)
    {
        oprintf(OUT_BST, 20, "\n\n-- #%d --\n", i + 1);
        
        oprintf(OUT_BST, 20, "                    hits: %d\n",
                run_stats[0].best[i]->ind->hits);
        oprintf(OUT_BST, 20, "             raw fitness: %.*lf\n",
                fd, run_stats[0].best[i]->ind->r_fitness);
        oprintf(OUT_BST, 20, "    standardized fitness: %.*lf\n",
                fd, run_stats[0].best[i]->ind->s_fitness);
        oprintf(OUT_BST, 20, "        adjusted fitness: %.*lf\n",
                fd, run_stats[0].best[i]->ind->a_fitness);
        
        oprintf(OUT_HIS, 20, "\n\n-- #%d --\n", i + 1);
        
        oprintf(OUT_HIS, 20, "                    hits: %d\n",
                run_stats[0].best[i]->ind->hits);
        oprintf(OUT_HIS, 20, "             raw fitness: %.*lf\n",
                fd, run_stats[0].best[i]->ind->r_fitness);
        oprintf(OUT_HIS, 20, "    standardized fitness: %.*lf\n",
                fd, run_stats[0].best[i]->ind->s_fitness);
        oprintf(OUT_HIS, 20, "        adjusted fitness: %.*lf\n",
                fd, run_stats[0].best[i]->ind->a_fitness);
        
        /* print the tree to both files here. */
        if (test_detail_level(20))
        {
            pretty_print_individual(run_stats[0].best[i]->ind, bout);
            pretty_print_individual_equ(run_stats[0].best[i]->ind, bout);
            pretty_print_individual(run_stats[0].best[i]->ind, hout);
        }
    }
    
    /* call the end-of-evaluation callback.  returns 1 if user termination
       criterion is met, 0 otherwise. */
    ret = app_end_of_evaluation(gen, mpop, newbest, gen_stats, run_stats);
    
    /* close the .bst file. */
    output_stream_close(OUT_BST);
    
    /* free stats structures for current generation. */
    for (i = 0; i < mpop->size + 1; ++i)
    {
        for (j = 0; j < gen_stats[i].bestn; ++j)
            --gen_stats[i].best[j]->refcount;
        FREE(gen_stats[i].best);
    }
    FREE(gen_stats);
    
    /* deallocate saved individuals that are no longer needed. */
    saved_individual_gc();
    
    /* return value the application callback gave us. */
    return ret;
    
}

/* evaluate_pop()
 *
 * evaluates all the individuals in a population whose cached
 * fitness values are invalid.
 */

void evaluate_pop(population* pop)
{
    int i;
#if defined(POSIX_MT) || defined(SOLARIS_MT)
    int start, end, inc, err;
    struct thread_param_t* t_param;
#endif
#if POSIX_MT
    pthread_t* t_ids;
#endif

#ifdef DEBUG
    print_individual ( pop->ind, stdout );
#ifdef COEVOLUTION
  printf("Can't do COEVOLUTION and DEBUG at the same time, sorry!\n");
#else
    app_eval_fitness ( pop->ind );
#endif
    exit(0);
#endif

#if !defined(POSIX_MT) && !defined(SOLARIS_MT)
#ifdef COEVOLUTION
    
    if (pop->size % 2)
    {
        /* It's not even! */
          error ( E_FATAL_ERROR, "Population must be even to do COEVOLUTION\n");
    }
    
      for ( i = 0; i < pop->size; i+=2)
        if ( pop->ind[i].evald != EVAL_CACHE_VALID ||
             pop->ind[i+1].evald != EVAL_CACHE_VALID )
          app_eval_fitness ( (pop->ind)+i );
#else
      for ( i = 0; i < pop->size; ++i )
        if ( pop->ind[i].evald != EVAL_CACHE_VALID )
          app_eval_fitness ( (pop->ind)+i );
#endif

#else

#if POSIX_MT
    t_ids = (pthread_t*) MALLOC(numthreads * sizeof(pthread_t));
#endif
    t_param = (struct thread_param_t*) MALLOC(numthreads *
                                              sizeof(struct thread_param_t));
    
    /* figure out how many pop members per thread */
    inc = pop->size / numthreads;
    if (pop->size != inc * numthreads) inc++;
    
    start = 0;
    for (i = 0; i < numthreads; i++)
    {
        end = start + inc;
        if (end > pop->size) end = pop->size;
        
        /* setup the paramater to pass */
        t_param[i].pop = pop;
        t_param[i].startidx = start;
        t_param[i].endidx = end;
        t_param[i].g = *(get_globaldata());

#ifdef POSIX_MT
        err = pthread_create(&t_ids[i], &pthread_attr,
                             evaluate_pop_chunk, &t_param[i]);
#endif
#ifdef SOLARIS_MT
        err = thr_create(NULL, (int)NULL, evaluate_pop_chunk,
                 &t_param[i], (int)NULL,NULL);
#endif
        if (err != 0)
        {
            error(E_FATAL_ERROR, "cannot create thread");
        }
        
        start = end;
    }

#ifdef SOLARIS_MT
    while (thr_join(NULL, NULL, NULL) == 0);
#endif
#ifdef POSIX_MT
    for (i = 0; i < numthreads; i++)
    {
        pthread_join(t_ids[i], NULL);
    }
    FREE(t_ids);
#endif
    FREE(t_param);

#endif

}

/* calculate_pop_stats()
 *
 * tabulates stats for a population:  fitness and size of best, worst,
 * mean, etc.  also finds top N individuals and saves them.
 */

void calculate_pop_stats(popstats* s, population* pop, int gen,
                         int subpop)
{
    int i, j, k, l;
    int b;
    saved_ind* shp;
    individual** temp;
    
    /* allocate a list of the top N individuals. */
    s->best = (saved_ind**) MALLOC(s->bestn *
                                   sizeof(saved_ind*));
    temp = (individual**) MALLOC((s->bestn + 1) * sizeof(individual*));
    
    s->size = pop->size;
    
    /** this is all pretty obvious -- set all the max and min values to the
      first individual's values, then go through the population looking for
      things that are bigger/smaller/better/worse/etc. **/
    
    s->maxnodes = s->minnodes = s->totalnodes = s->bestnodes = s->worstnodes =
            individual_size(pop->ind + 0);
    s->maxdepth = s->mindepth = s->totaldepth = s->bestdepth = s->worstdepth =
            individual_depth(pop->ind + 0);
    s->maxhits = s->minhits = s->totalhits = s->besthits = s->worsthits =
            pop->ind[0].hits;
    s->bestfit = s->worstfit = s->totalfit = pop->ind[0].a_fitness;
    temp[0] = pop->ind;
    b = 1;
    s->bestgen = s->worstgen = gen;
    s->bestpop = s->worstpop = subpop;
    
    for (i = 1; i < s->size; ++i)
    {
        j = individual_size(pop->ind + i);
        s->totalnodes += j;
        if (j < s->minnodes) s->minnodes = j;
        if (j > s->maxnodes) s->maxnodes = j;
        
        k = individual_depth(pop->ind + i);
        s->totaldepth += k;
        if (k < s->mindepth) s->mindepth = k;
        if (k > s->maxdepth) s->maxdepth = k;
        
        l = pop->ind[i].hits;
        s->totalhits += l;
        if (l < s->minhits) s->minhits = l;
        if (l > s->maxhits) s->maxhits = l;
        
        s->totalfit += pop->ind[i].a_fitness;
        if (pop->ind[i].a_fitness > s->bestfit)
        {
            s->bestfit = pop->ind[i].a_fitness;
            s->bestnodes = j;
            s->bestdepth = k;
            s->besthits = l;
        } else if (pop->ind[i].a_fitness < s->worstfit)
        {
            s->worstfit = pop->ind[i].a_fitness;
            s->worstnodes = j;
            s->worstdepth = k;
            s->worsthits = l;
        }
        
        /** insert the current individual into the top N list
      (if it belongs there). **/
        
        for (j = b; j > 0; --j)
        {
            if (pop->ind[i].a_fitness < temp[j - 1]->a_fitness)
                break;
            temp[j] = temp[j - 1];
        }
        if (j < s->bestn)
            temp[j] = pop->ind + i;
        if (b < s->bestn)
            ++b;
    }
    
    /** now save copies of the individuals in the "temp" list **/
    for (i = 0; i < b; ++i)
    {
        shp = (saved_ind*) MALLOC(sizeof(saved_ind));
        shp->ind = (individual*) MALLOC(sizeof(individual));
        shp->ind->tr = (tree*) MALLOC(tree_count * sizeof(tree));
        duplicate_individual(shp->ind, temp[i]);
        for (j = 0; j < tree_count; ++j)
            reference_ephem_constants(shp->ind->tr[j].data, 1);
        shp->refcount = 1;
        shp->next = NULL;
        
        saved_tail->next = shp;
        saved_tail = shp;
        ++saved_head->refcount;
        
        s->best[i] = shp;
    }

#ifdef DEBUG
    printf ( "the best list is:\n" );
    for ( j = 0; j < s->bestn; ++j )
      printf ( "     %08x  %lf\n", s->best[j], s->best[j]->ind->a_fitness );
#endif
    
    FREE(temp);
    
}

/* accumulate_pop_stats()
 *
 * this merges the second statistics record into the first, so that it reflects
 * the "sum" of the underlying populations.  returns 1 if the best individual
 * of the first record has changed (that is, if the second record has a better
 * best individual. 
 */

int accumulate_pop_stats(popstats* total, popstats* n)
{
    int ret = 0;
    int i, j, k;
    saved_ind** temp;
    
    if (total->size == -1)
    {
        /* if the "total" record is empty, then just copy the second record
       into it. */
        memcpy(total, n, sizeof(popstats));
        total->best = (saved_ind**) MALLOC(total->bestn *
                                           sizeof(saved_ind*));
        memcpy(total->best, n->best, total->bestn *
                                     sizeof(saved_ind*));
        ret = 1;
    } else
    {
        /* sum the totals. */
        total->size += n->size;
        total->totalnodes += n->totalnodes;
        total->totaldepth += n->totaldepth;
        total->totalhits += n->totalhits;
        total->totalfit += n->totalfit;
        
        /* find the maximums. */
        if (n->maxnodes > total->maxnodes) total->maxnodes = n->maxnodes;
        if (n->maxdepth > total->maxdepth) total->maxdepth = n->maxdepth;
        if (n->maxhits > total->maxhits) total->maxhits = n->maxhits;
        
        /* find the minimums. */
        if (n->minnodes < total->minnodes) total->minnodes = n->minnodes;
        if (n->mindepth < total->mindepth) total->mindepth = n->mindepth;
        if (n->minhits < total->minhits) total->minhits = n->minhits;
        
        /* find the best individual's numbers. */
        if (n->bestfit > total->bestfit)
        {
            total->bestfit = n->bestfit;
            total->bestnodes = n->bestnodes;
            total->bestdepth = n->bestdepth;
            total->besthits = n->besthits;
            total->bestgen = n->bestgen;
            total->bestpop = n->bestpop;
            ret = 1;
        }
        
        /* find the worst individual's numbers. */
        if (n->worstfit < total->worstfit)
        {
            total->worstfit = n->worstfit;
            total->worstnodes = n->worstnodes;
            total->worstdepth = n->worstdepth;
            total->worsthits = n->worsthits;
            total->worstgen = n->worstgen;
            total->worstpop = n->worstpop;
        }

#ifdef DEBUG
        printf ( "total list:\n" );
        for ( i = 0; i < total->bestn; ++i )
      printf ( "   %08x %lf\n",
           total->best[i], total->best[i]->ind->a_fitness );
        printf ( "new list:\n" );
        for ( i = 0; i < n->bestn; ++i )
      printf ( "   %08x %lf\n",
           n->best[i], n->best[i]->ind->a_fitness );
#endif
        
        /** here we merge the two "top N" lists into one, discarding
      the remaining N individuals. **/
        
        temp = (saved_ind**) MALLOC(total->bestn *
                                    sizeof(saved_ind*));
        j = 0;   /* position in "total"s list */
        k = 0;   /* position in "n"s list */
        for (i = 0; i < total->bestn; ++i)
        {
            /* if the n list is empty, take from the total list. */
            if (k == -1)
                temp[i] = total->best[j++];
                /* if the total list is empty, take from the n list. */
            else if (j == -1)
            {
                ret |= (i == 0);
                temp[i] = n->best[k++];
            }
                /* if neither list is empty, take the better individual. */
            else if (total->best[j]->ind->a_fitness <
                     n->best[k]->ind->a_fitness)
            {
                ret |= (i == 0);
                temp[i] = n->best[k++];
            } else
                temp[i] = total->best[j++];
            
            /* have we run off the end of either list? */
            if (j >= total->bestn)
                j = -1;
            if (k >= n->bestn)
                k = -1;
        }
        
        /* decrement the reference count of the old "best" list. */
        for (i = 0; i < total->bestn; ++i)
            --total->best[i]->refcount;
        
        FREE(total->best);
        total->best = temp;

#ifdef DEBUG
        printf ( "new total list:\n" );
        for ( i = 0; i < total->bestn; ++i )
      printf ( "   %08x %lf\n",
           total->best[i], total->best[i]->ind->a_fitness );
#endif
    }
    
    /* increment the reference count of the new "best" list. */
    for (i = 0; i < total->bestn; ++i)
        ++total->best[i]->refcount;
    
    return ret;
}

/* saved_individual_gc()
 *
 * go through the list of saved individuals, deleting any which are no longer
 * referred to. 
 */

void saved_individual_gc(void)
{
    int j;
    saved_ind* shp = saved_head->next;
    saved_ind* shm = saved_head;
    
    while (shp)
    {
        if (shp->refcount == 0)
        {
            /** found one that needs to be deleted. **/
            
            /* dereference its trees' ERCs and delete the trees. */
            for (j = 0; j < tree_count; ++j)
            {
                reference_ephem_constants(shp->ind->tr[j].data, -1);
                free_tree(shp->ind->tr + j);
            }
            FREE(shp->ind->tr);
            FREE(shp->ind);
            
            /* cut the record out of the linked list. */
            shm->next = shp->next;
            if (saved_tail == shp)
                saved_tail = shm;
            FREE(shp);
            shp = shm->next;
            
            /* the refcount field of the list head (a dummy node) holds the
               size of the list. */
            --saved_head->refcount;
        } else
        {
            /* move down the list. */
            shm = shp;
            shp = shp->next;
        }
    }
}

/* read_saved_individuals()
 *
 * reads the list of saved individuals from a checkpoint file.  constructs
 * an index translating indices to addresses.
 */

saved_ind** read_saved_individuals(ephem_const** eind, FILE* f)
{
    char* buffer;
    int count;
    int i;
    saved_ind* p;
    saved_ind** sind;
    
    buffer = (char*) MALLOC(MAXCHECKLINELENGTH);
    
    /* read the number of saved individuals. */
    fscanf(f, "%*s %d\n", &count);
    
    /* allocate the index. */
    sind = (saved_ind**) MALLOC(count * sizeof(saved_ind*));
    
    /* allocate the head of the linked list (a dummy node whose refcount
       equals the number of individuals on the list). */
    saved_head = (saved_ind*) MALLOC(sizeof(saved_ind));
    saved_head->ind = NULL;
    saved_head->refcount = count;
    p = saved_head;
    for (i = 0; i < count; ++i)
    {
        /* allocate the next saved_ind on the list. */
        p->next = (saved_ind*) MALLOC(sizeof(saved_ind));
        p = p->next;
        /* allocate the individual. */
        p->ind = (individual*) MALLOC(sizeof(individual));
        /* make the index entry. */
        sind[i] = p;
        /* read the refcount. */
        fscanf(f, "%d ", &(p->refcount));
        /* read the individual. */
        read_individual(p->ind, eind, f, buffer);
    }
    /* mark the end of the list. */
    p->next = NULL;
    saved_tail = p;
    
    FREE(buffer);
    
    return sind;
}

/* read_stats_checkpoint()
 *
 * read the overall run statistics structures from a checkpoint file.
 */

void read_stats_checkpoint(multipop* mpop, ephem_const** eind, FILE* f)
{
    int i, j, k;
    saved_ind** sind;
    
    /* read and index the saved individuals list. */
    sind = read_saved_individuals(eind, f);
    
    /* allocate the run_stats array. */
    run_stats = (popstats*) MALLOC((mpop->size + 1) * sizeof(popstats));
    for (i = 0; i < mpop->size + 1; ++i)
    {
        /* read lots of integer values into run_stats. */
        fscanf(f, "%d  %d %d %d %d %d  %d %d %d %d %d  %d %d %d %d %d\n",
               &(run_stats[i].size),
               &(run_stats[i].maxnodes), &(run_stats[i].minnodes),
               &(run_stats[i].totalnodes), &(run_stats[i].bestnodes),
               &(run_stats[i].worstnodes),
               &(run_stats[i].maxdepth), &(run_stats[i].mindepth),
               &(run_stats[i].totaldepth), &(run_stats[i].bestdepth),
               &(run_stats[i].worstdepth),
               &(run_stats[i].maxhits), &(run_stats[i].minhits),
               &(run_stats[i].totalhits), &(run_stats[i].besthits),
               &(run_stats[i].worsthits));
        /** double-precision values are stored as hex data to avoid loss
      of precision. **/
        read_hex_block(&(run_stats[i].bestfit), sizeof(double), f);
        fgetc(f);
        read_hex_block(&(run_stats[i].worstfit), sizeof(double), f);
        fgetc(f);
        read_hex_block(&(run_stats[i].totalfit), sizeof(double), f);
        /* they are also printed as decimal values, for the benefit of human
       readers -- skip these fields. */
        fscanf(f, " %*f %*f %*f\n");
        /* read some more integers. */
        fscanf(f, "%d %d %d %d %d ",
               &(run_stats[i].bestgen), &(run_stats[i].worstgen),
               &(run_stats[i].bestpop), &(run_stats[i].worstpop),
               &(run_stats[i].bestn));
        run_stats[i].best = (saved_ind**) MALLOC(run_stats[i].bestn *
                                                 sizeof(saved_ind*));
        /** read the indices of the contents of the best array, and look up
      the addresses in the index. **/
        for (j = 0; j < run_stats[i].bestn; ++j)
        {
            fscanf(f, "%d\n", &k);
            run_stats[i].best[j] = sind[k];
        }
    }
    FREE(sind);
}

/* write_saved_individuals()
 *
 * writes the linked list of saved individuals to a checkpoint file.  returns
 * an index for translating saved_ind addresses to integer indices.
 */

saved_ind** write_saved_individuals(ephem_index* eind, FILE* f)
{
    saved_ind** index;
    saved_ind* shp;
    int i = 0;
    
    index = (saved_ind**) MALLOC(saved_head->refcount *
                                 sizeof(saved_ind*));
    
    /* write the count of individuals. */
    fprintf(f, "saved-individual-count: %d\n", saved_head->refcount);
    
    shp = saved_head->next;
    
    /** traverse the linked list. **/
    while (shp)
    {
        /* write the reference count and individual. */
        fprintf(f, "%d ", shp->refcount);
        write_individual(shp->ind, eind, f);
        
        /* record the address in the index. */
        index[i++] = shp;
        
        shp = shp->next;
    }
    
    return index;
}

/* write_stats_checkpoint()
 *
 * write the overall run statistics structures to a checkpoint file.
 */

void write_stats_checkpoint(multipop* mpop, ephem_index* eind, FILE* f)
{
    int i, j, k;
    saved_ind** sind;
    
    /* write and index the saved individuals list. */
    sind = write_saved_individuals(eind, f);
    
    for (i = 0; i < mpop->size + 1; ++i)
    {
        /* write many integer values. */
        fprintf(f, "%d  %d %d %d %d %d  %d %d %d %d %d  %d %d %d %d %d\n",
                run_stats[i].size,
                run_stats[i].maxnodes, run_stats[i].minnodes,
                run_stats[i].totalnodes, run_stats[i].bestnodes,
                run_stats[i].worstnodes,
                run_stats[i].maxdepth, run_stats[i].mindepth,
                run_stats[i].totaldepth, run_stats[i].bestdepth,
                run_stats[i].worstdepth,
                run_stats[i].maxhits, run_stats[i].minhits,
                run_stats[i].totalhits, run_stats[i].besthits,
                run_stats[i].worsthits);
        /** write double-precision values as hex data. **/
        write_hex_block(&(run_stats[i].bestfit), sizeof(double), f);
        fputc(' ', f);
        write_hex_block(&(run_stats[i].worstfit), sizeof(double), f);
        fputc(' ', f);
        write_hex_block(&(run_stats[i].totalfit), sizeof(double), f);
        /* also write them as decimal values. */
        fprintf(f, " %f %f %f\n", run_stats[i].bestfit,
                run_stats[i].worstfit, run_stats[i].totalfit);
        /* write more integers. */
        fprintf(f, "%d %d %d %d %d ",
                run_stats[i].bestgen, run_stats[i].worstgen,
                run_stats[i].bestpop, run_stats[i].worstpop,
                run_stats[i].bestn);
        /** write the best array, indexing saved individuals using integers. **/
        for (j = 0; j < run_stats[i].bestn; ++j)
        {
            /** search the index for the address. **/
            for (k = 0; k < saved_head->refcount; ++k)
                if (run_stats[i].best[j] == sind[k])
                {
                    /* print the index to the checkpoint file. */
                    fprintf(f, " %d", k);
                    break;
                }
            /** address was not found in the index. **/
            if (k == saved_head->refcount)
            {
                /* this shouldn't ever happen. */
                fprintf(f, " -1");
                error(E_WARNING, "bestn pointer is bad.");
            }
        }
        
        fputc('\n', f);
    }
    
    FREE(sind);
}


#if !defined(POSIX_MT) && !defined(SOLARIS_MT)

/* return the globaldata structure */
globaldata *get_globaldata(void) {
  return( &global_g );
}

#else /* continues to end of file */

/* provide each thread with seperate copy of 'g' */
globaldata* get_globaldata(void)
{
    globaldata* retval;

#ifdef POSIX_MT
    retval = pthread_getspecific(g_key);
#endif
#ifdef SOLARIS_MT
    thr_getspecific( g_key, (void *)&retval );
#endif
    
    if (retval == NULL)
    {
        error(E_FATAL_ERROR, "get_globaldata() tried to return NULL");
    }
    
    return (retval);
}


/*
 * initialize_threading()
 *
 * Setup the program for multithreading use.
 */

void initialize_threading(void)
{
    char* numthreads_str;
    globaldata* main_g;
    
    /* numthreads_str = getenv( "NUM_THREADS" );*/
    numthreads_str = get_parameter("num_threads"); /* Now in input file */
    if (numthreads_str == NULL)
    {
        error(E_FATAL_ERROR, "num_threads undefined");
    }
    numthreads = atoi(numthreads_str);
    if (numthreads < 1)
    {
        error(E_FATAL_ERROR, "num_threads must be > 0");
    }
    
    printf("numthreads: %d\n", numthreads);
    
    /* create the thread key for access to 'g' */
#ifdef POSIX_MT
    pthread_key_create(&g_key, NULL);
#endif
#ifdef SOLARIS_MT
    thr_keycreate( &g_key, NULL );
#endif
    
    /* main thread needs its own 'g' */
    main_g = (globaldata*) malloc(sizeof(globaldata));
#ifdef POSIX_MT
    pthread_setspecific(g_key, main_g);
#endif
#ifdef SOLARIS_MT
    thr_setspecific(g_key, main_g);
#endif

#ifdef POSIX_MT
    /* Setup default thread attributes */
    pthread_attr_init(&pthread_attr);
    pthread_attr_setscope(&pthread_attr, PTHREAD_SCOPE_SYSTEM);
    
    if (1)
    {
        size_t size;
        size_t guard;
        
        /* Double the stack size */
        pthread_attr_getstacksize(&pthread_attr, &size);
        printf("Kernel Old Size %d\n", (int) size);
        pthread_attr_setstacksize(&pthread_attr, size * 2);
        pthread_attr_getstacksize(&pthread_attr, &size);
        printf("Kernel New Size %d\n", (int) size);

/* Double the Guard size (account for old pthread implementation) */
#ifdef PTHREAD_1.X
        pthread_attr_getguardsize_np(&pthread_attr,&guard);
        printf("Kernel Old Guard %d\n",(int)guard);
        pthread_attr_setguardsize_np(&pthread_attr,2*guard);
        pthread_attr_getguardsize_np(&pthread_attr,&guard);
#else
        pthread_attr_getguardsize(&pthread_attr, &guard);
        printf("Kernel Old Guard %d\n", (int) guard);
        pthread_attr_setguardsize(&pthread_attr, 2 * guard);
        pthread_attr_getguardsize(&pthread_attr, &guard);
#endif
        printf("Kernel New Guard %d\n", (int) guard);
    }
#endif

#ifdef SOLARIS_MT
    /* let the OS know how many threads to run at once */
    thr_setconcurrency( numthreads );
#endif

}

/* evaluate_pop_chuck()
 *
 * Called from evaluate_pop to do a chunk of evaluations on a pop.
 * This was done to allow multithreading.
 */

void* evaluate_pop_chunk(void* param)
{
    int k, startidx, endidx;
    population* pop;
    globaldata* g;
    struct thread_param_t* t_param;
    
    t_param = (struct thread_param_t*) param;
    pop = t_param->pop;
    startidx = t_param->startidx;
    endidx = t_param->endidx;

#ifdef POSIX_MT
    pthread_setspecific(g_key, &t_param->g);
#endif
#ifdef SOLARIS_MT
    thr_setspecific(g_key, &t_param->g);
#endif
    
    g = get_globaldata();
    
    /* printf("START: %d,%d\n", startidx, endidx); */

#ifdef COEVOLUTION            /* Here we hack it to provide *two* individuals */
    
    if ((endidx-startidx)%2)  /* it it's not even */
  
      {
        char xx[256];
        sprintf(xx,"Uneven number of individuals (%d) at %d",
            endidx-startidx,startidx);
        error ( E_FATAL_ERROR,xx);
      }
  
    for ( k = startidx; k < endidx; k+=2 )
      
      /* It's GOT to have even number of inds */
  
      if ( pop->ind[k].evald != EVAL_CACHE_VALID ||
       pop->ind[k+1].evald != EVAL_CACHE_VALID )
        {
      app_eval_fitness ( (pop->ind)+k, (pop->ind)+(k+1) );
        }
#else
    for (k = startidx; k < endidx; ++k)
        if (pop->ind[k].evald != EVAL_CACHE_VALID)
        {
            app_eval_fitness((pop->ind) + k);
        }
#endif

}

#endif /* !defined(POSIX_MT) && !defined(SOLARIS_MT) */


