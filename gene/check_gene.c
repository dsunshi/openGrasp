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

#include <stdio.h>
#include <stdlib.h>
#define DEFAULT_GENE_SIZE 64

const guint8 solution[DEFAULT_GENE_SIZE] =
{
    '1', '1', '1', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
    '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
    '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
    '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '1', '1', '1'
};

gint calc_indv_fitness(Individual *indv)
{
    gint fitness = 0;
    gint index;
    
    for (index = 0; index < indv->size; index++)
    {
        if (indv->genes[index] == solution[index])
        {
            fitness++;
        }
        else
        {
        }
    }
    
    return fitness;
}

void print_indv (Individual  *indv)
{
    gint index;
    
    printf("Fitness: %d\n", indv->fitness);
    printf("Address: %X\n", (unsigned int)((intptr_t) indv));
    printf("Genes:   ");
    
    for (index = 0; index < indv->size; index++)
    {
        printf("%c ", indv->genes[index]);
    }
    
    printf("\n");
}

int main()
{
	GeneticSolver *gs;
	Individual    *fittest;
	
	gs = gs_create();
	
	if (gs == NULL)
	{
		printf ("Failed to create GeneticSolver\n");
	}
	else
	{
		gs->max_fitness      = DEFAULT_GENE_SIZE;
		gs->gene_size        = DEFAULT_GENE_SIZE;
		gs->population_size  = 100;
        gs->tournament_size  = 5;
		gs->fitness_function = calc_indv_fitness;
		
		fittest = gs_solve(gs, 10000);
		
		print_indv( fittest );
		
		destroy_gs( gs );
	}
	
	return 0;
}
