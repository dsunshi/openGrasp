/*
 * Copyright (c) 2015 David Sunshine, <http://sunshin.es>
 * 
 * This file is part of openGrasp.
 * 
 * openGrasp is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 * 
 * openGrasp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with openGrasp.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _GENE_H_
#define _GENE_H_

#include <glib.h>

/* Temporary debugging */
#include <stdio.h>

#include "../atree/atree.h"

#define UNIFORM_RATE                ((guint32) (0.5   * G_MAXUINT32))
#define MUTATION_RATE               ((guint32) (0.015 * G_MAXUINT32))
#define ELITISM                     FALSE

#if (ELITISM != FALSE)
  #error('Feature is not currently supported!\n');
#endif

typedef struct individual_t
{
    guint8 *genes;
    gint    fitness;
	
	gint    size;
	/* Fitness Function: return gint with Individual argument */
    gint    (*fitness_function) (struct individual_t *indv);
	GRand    *random;
} Individual;

typedef struct population_t
{
    ATree *individuals;
    gint   size;
	gint   tournament_size;
} Population;

typedef struct genetic_solver_t
{
    /* Internal parameters */
	Population *pop;
    GRand      *random;
    
    /* Results */
    Individual *fittest;
    
    /* User defined parameters */
	gint        population_size;
	gint        gene_size;
	gint        tournament_size;
	/* Fitness Function: return gint with Individual argument */
    gint      (*fitness_function) (Individual *indv);
	gint        max_fitness;
} GeneticSolver;

GeneticSolver* gs_create  (void);
Individual*    gs_solve   (GeneticSolver* gs, gint limit);

void           destroy_gs (GeneticSolver* gs);
        
#define GSOLVER_SET_MAX_FITNESS(g,x)        ((g)->max_fitness      = (x))
#define GSOLVER_SET_GENE_SIZE(g,x)          ((g)->gene_size        = (x))
#define GSOLVER_SET_POP_SIZE(g,x)           ((g)->population_size  = (x))
#define GSOLVER_SET_TOURNAMENT_SIZE(g,x)    ((g)->tournament_size  = (x))
#define GSOLVER_SET_FITNESS_FUNC(g,x)       ((g)->fitness_function = (x))

#endif
