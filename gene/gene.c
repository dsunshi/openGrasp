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

#include "gene.h"

/* time.h, needed for seeding the random number generator */
#include <time.h>

static Population *generate_pop           (GeneticSolver *gs);
static Individual *generate_individual    (GeneticSolver *gs);
static void        destroy_pop            (Population *pop);
static void        destory_individual     (Individual *indv);
static void        set_pop_indv           (Population *pop, Individual *indv);
static Individual *get_pop_rand_indv      (Population *pop);
static void        reset_individual       (Individual *indv);
static gint        get_indv_fitness       (Individual *indv);
static guint8      get_indv_gene          (Individual *indv, gint gene);
static void        set_indv_gene          (Individual *indv, gint gene, guint8 value);
static void        mutate_indv            (Individual *indv);
static void        crossover_indv         (Individual *a, Individual *b, Individual *crossed);
static Individual *tournament_select_indv (Population *pop, gint size);
static Individual *get_pop_fittest        (Population *pop);
static Individual *get_pop_weakest        (Population *pop);
static Population *evolve_pop             (Population *pop);

/* Compatibility for GLIB 2.38 */
#ifndef g_assert_nonnull
  #define g_assert_nonnull(x) g_assert((x) != NULL)
#endif

typedef struct population_set_t
{
	Population *old;
	Population *new;
} PopulationSet;

static void iterate_pop (gpointer data, gpointer user_data)
{
	Individual *a = NULL;
    Individual *b = NULL;
    Individual *c = (Individual *) data;
	
	PopulationSet *set = (PopulationSet *) user_data;
    
    g_assert_nonnull( data );
    g_assert_nonnull( user_data );
	
	a = tournament_select_indv(set->old, (set->old)->tournament_size);
    b = tournament_select_indv(set->old, (set->old)->tournament_size);

	crossover_indv(a, b, c);
	
	mutate_indv(c);
	
	set_pop_indv(set->new, c);
}

Population *evolve_pop (Population *pop)
{
    Population *new_pop = NULL;
    GPtrArray  *list    = NULL;
    Individual *a       = NULL;
	
	PopulationSet set;

    g_assert_nonnull( pop );
    
    new_pop = (Population *) g_new(Population, 1);
    
    if (new_pop == NULL)
    {
        g_error("GeneticSolver-CRITICAL **: Could not allocate memory for Population\n");
    }
    else
    {
        new_pop->individuals     = a_tree_new();
        new_pop->size            = pop->size;
        new_pop->tournament_size = pop->tournament_size;
		
		set.old = pop;
		set.new = new_pop;

        if (ELITISM != FALSE)
        {
            a = get_pop_fittest(pop);
            set_pop_indv(new_pop, a);
        }
        else
        {
            /* Do Nothing */
        }
        
        list = a_tree_list_all( pop->individuals );
		g_ptr_array_foreach( list, iterate_pop, &set );
    }

    destroy_pop(pop);
    
    g_assert_nonnull( new_pop );

    return new_pop;
}

GeneticSolver* gs_create (void)
{
	GeneticSolver* gs = NULL;
	
	gs = (GeneticSolver *) g_new( GeneticSolver, 1 );
	
	if (gs == NULL)
	{
		g_error("GeneticSolver-CRITICAL **: Could not allocate memory for Genetic Solver\n");
	}
	else
	{
		gs->random = g_rand_new();
	}
	
    g_assert_nonnull( gs );
    
	return gs;
}

void destroy_gs (GeneticSolver* gs)
{
	/* Todo: Destroy all individuals */
	g_free( gs );
}

Individual* gs_solve (GeneticSolver* gs, gint limit)
{
	Population *pop      = NULL;
    Individual *fittest  = NULL;
    
    gint generation_count;
    
    g_assert_nonnull( gs );

    pop = generate_pop( gs );

    for (generation_count = 0; generation_count < limit; generation_count++)
    {
        fittest = get_pop_fittest(pop);
        
        if (get_indv_fitness(fittest) >= gs->max_fitness)
        {
            printf("Generation:\t%d\n", generation_count);
            break;
        }
        else
        {
            pop = evolve_pop(pop);
        }
    }
    
    g_assert_nonnull( fittest );

    return fittest;
}

static Population *generate_pop (GeneticSolver *gs)
{
    gint index;
	gint length      = gs->population_size;
    
    Individual *indv = NULL;
    Population *pop  = NULL;
    
    g_assert_nonnull( gs );
    
    pop = g_new(Population, 1);
    
    if (pop == NULL)
    {
        g_error("GeneticSolver-CRITICAL **: Could not allocate memory for Population\n");
    }
    else
    {
		pop->individuals = a_tree_new();
		
        for (index = 0; index < length; index++)
        {
			indv = generate_individual( gs );
			set_pop_indv(pop, indv);
        }
        
        pop->size            = length;
		pop->tournament_size = gs->tournament_size;
    }
    
    g_assert_nonnull( pop );
    
    return pop;
}

static Individual *generate_individual (GeneticSolver *gs)
{
	Individual *indv = NULL;
	
	indv = g_new(Individual, 1);
    
    g_assert_nonnull( gs );
    
    if (indv == NULL)
    {
        g_error("GeneticSolver-CRITICAL **: Could not allocate memory for Individual\n");
    }
    else
    {
		indv->genes = g_new(guint8, gs->gene_size);
		
		if (indv->genes == NULL)
		{
			g_error("GeneticSolver-CRITICAL **: Could not allocate memory for Individual\n");
		}
		else
		{			
			indv->size             = gs->gene_size;
			indv->random           = gs->random;
			indv->fitness_function = gs->fitness_function;
			
			reset_individual(indv);
		}
    }
    
    g_assert_nonnull( indv );
	
	return indv;
}

static void reset_individual (Individual *indv)
{
	gint index;
    
    g_assert_nonnull( indv );
	
	for (index = 0; index < indv->size; index++)
	{
		indv->genes[index] = g_rand_int_range( indv->random, 0, G_MAXUINT8 );
	}
	
	indv->fitness = 0;
}

static void destroy_pop (Population *pop)
{
    g_assert_nonnull( pop );
    
	a_tree_destroy ( pop->individuals );
	g_free( pop );
}

static void destory_individual (Individual *indv)
{
    g_assert_nonnull( indv );
    
	g_free( indv->genes );
	g_free( indv );
}

static void set_pop_indv (Population *pop, Individual *indv)
{
    gint key = get_indv_fitness(indv);
    
    g_assert_nonnull( pop );
    g_assert_nonnull( indv );
    
    atree_insert( pop->individuals, GINT_TO_POINTER(&key), indv);
}

static Individual *get_pop_rand_indv (Population *pop)
{
    Individual *random_indv;
    
    g_assert_nonnull( pop );

	random_indv = (Individual *) atree_random_value( pop->individuals );
    
    g_assert_nonnull( random_indv );
    
    return random_indv;
}

static gint get_indv_fitness (Individual *indv)
{
    gint fitness;
    
    if (indv == NULL)
    {
        fitness = 0;
    }
    else
    {        
        fitness = indv->fitness;
        
        if (fitness == 0)
        {
            /* Calculate the fitness for this individual */
            fitness = indv->fitness_function(indv);
        }
        else
        {
            /* Do nothing, return the previously calculated fitness */
        }
        
        indv->fitness = fitness;
    }
    
    return fitness;
}

static guint8 get_indv_gene (Individual *indv, gint gene)
{
    g_assert_nonnull( indv ); 
    
    return (indv->genes[gene]);
}

static void set_indv_gene (Individual *indv, gint gene, guint8 value)
{
    g_assert_nonnull( indv ); 
    
    indv->genes[gene] = value;
    indv->fitness     = 0;
}

static void mutate_indv (Individual *indv)
{
    gint index;
    
    g_assert_nonnull( indv );
    
    for (index = 0; index < indv->size; index++)
    {
        if ( g_rand_int(indv->random) <= MUTATION_RATE)
        {
			/* Mutate the current gene */
            indv->genes[index] = g_rand_int_range( indv->random, 0, G_MAXUINT8 );
        }
		else
		{
			/* The current gene is not mutated */
		}
    }
    
    indv->fitness = 0;
}

static void crossover_indv (Individual *a, Individual *b, Individual *crossed)
{
    guint8 gene;
    gint   index;
    
    g_assert_nonnull( a ); 
    g_assert_nonnull( b ); 
    g_assert_nonnull( crossed ); 
    
    for (index = 0; index < a->size; index++)
    {
        if ( g_rand_int(a->random) <= UNIFORM_RATE)
        {
            gene = get_indv_gene(a, index);
        }
        else
        {
            gene = get_indv_gene(b, index);
        }
        
        crossed->genes[index] = gene;
    }
    
    crossed->fitness = 0;
}

static Individual *tournament_select_indv (Population *pop, gint size)
{
    Individual *fittest     = NULL;
    Individual *random_indv = NULL;

    gint index;
    
    g_assert_nonnull( pop );
    
    for (index = 0; index < size; index++)
    {
        random_indv = get_pop_rand_indv(pop);
        
        if (get_indv_fitness(fittest) <= get_indv_fitness(random_indv))
        {
            fittest = random_indv;
        }
        else
        {
			/* Keep searching for a fitter individual */
        }
    }
    
    g_assert_nonnull( fittest );
    
    return fittest;
}

static Individual *get_pop_fittest (Population *pop)
{
    Individual *fittest;
    
    g_assert_nonnull( pop );
    
    fittest = (Individual *) atree_max_value( pop->individuals );
    
    g_assert_nonnull( fittest );
    
    return fittest;
}

static Individual *get_pop_weakest (Population *pop)
{
   Individual *weakest;
   
    g_assert_nonnull( pop );
    
    weakest = (Individual *) atree_min_value( pop->individuals );
    
    g_assert_nonnull( weakest );
    
	return weakest;
}
