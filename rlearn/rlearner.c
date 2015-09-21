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

#include "rlearner.h"

static void   init_qvalues    (RLearner *rl);
static int    best_action     (RLearner *rl, const unsigned int state);
static double best_qvalue     (RLearner *rl, const unsigned int state);
static void   update_qvalues  (RLearner *rl, const unsigned int prev_state,
                                             const unsigned int next_state, const unsigned int action, const double reward);
static int    semi_uniform    (RLearner *rl, const unsigned int state);
                              
static double ** malloc_2d_array (const unsigned int rows, const unsigned int cols);
static void      free_2d_array   (double **array, const unsigned int rows);

RLearner *r_learner_new (const unsigned int num_states, const unsigned int num_actions)
{
    RLearner *rl = NULL;
    
    rl = (RLearner *) malloc ( sizeof(RLearner) );
    
    if (rl == NULL)
    {
        fprintf(stderr, "RLearner-CRITICAL **: Could not allocate memory for RLearner\n");
    }
    else
    {
        rl->q_values    = malloc_2d_array( num_states, num_actions );
        rl->num_states  = num_states;
        rl->num_actions = num_actions;
        rl->random      = g_rand_new();
        rl->next_state  = NULL;
        rl->reward      = NULL;
        
        init_qvalues( rl );
    }
    
    return rl;
}

void r_learner_free (RLearner *rl)
{
    free_2d_array( rl->q_values, rl->num_states);
    free( rl );
}

static double ** malloc_2d_array (const unsigned int rows, const unsigned int cols)
{
    int      index = 0;
    double **array = NULL;
    
    array = malloc( rows * sizeof(double *) );
    
    if (array == NULL)
    {
        fprintf(stderr, "RLearner-CRITICAL **: Could not allocate memory for array\n");
    }
    else
    {
        for (index = 0; index < rows; index++)
        {
            array[index] = malloc( cols * sizeof(double) );
            
            if (array[index] == NULL)
            {
                fprintf(stderr, "RLearner-CRITICAL **: Could not allocate memory for array\n");
            }
            else
            {
            }
        }
    }
    
    return array;
}

static void free_2d_array (double **array, const unsigned int rows)
{
    int index = 0;

    for (index = 0; index < rows; index++)
    {
        free( array[index] );
    }
    
    free( array );
}

static void init_qvalues (RLearner *rl)
{
	int state  = 0;
	int action = 0;

    if (rl == NULL)
    {
        fprintf(stderr, "RLearner-WARNING **: RLearner *rl is NULL\n");
    }
    else
    {
        for (state = 0; state < rl->num_states; state++)
        {
            for (action = 0; action < rl->num_actions; action++)
            {
                rl->q_values[state][action] = 0.0;
            }
        }
    }
}

static int best_action (RLearner *rl, const unsigned int state)
{
	int action       = 0;
	int best_action  = 0;

    if (rl == NULL)
    {
        fprintf(stderr, "RLearner-WARNING **: RLearner *rl is NULL\n");
    }
    else
    {
        for (action = 0; action < rl->num_actions; action++)
        {
            if (rl->q_values[state][action] > rl->q_values[state][best_action])
            {
                best_action = action;
            }
            else
            {
                /* Not the best action */
            }
        }
	}
    
	return best_action;
}

static double best_qvalue (RLearner *rl, const unsigned int state)
{
    double best_value = 0;
    
    if (rl == NULL)
    {
        fprintf(stderr, "RLearner-WARNING **: RLearner *rl is NULL\n");
    }
    else
    {
        best_value = rl->q_values[ state ][ best_action(rl, state) ];
    }
    
	return best_value;
}

static void update_qvalues (RLearner *rl, const unsigned int prev_state,
                            const unsigned int next_state, const unsigned int action, const double reward)
{
	double best_qval = 0.0;
	double qval      = 0.0;
    
    if (rl == NULL)
    {
        fprintf(stderr, "RLearner-WARNING **: RLearner *rl is NULL\n");
    }
    else
    {
        best_qval = best_qvalue( rl, next_state );
        qval      = rl->q_values[prev_state][action];
        
        rl->q_values[prev_state][action] = (1 - BETA) * qval + BETA * (reward  + GAMMA * best_qval);
    }
}

static int semi_uniform (RLearner *rl, const unsigned int state)
{
	double rand_value = 0.0; 
	int    action     = 0;
	
    if (rl == NULL)
    {
        fprintf(stderr, "RLearner-WARNING **: RLearner *rl is NULL\n");
    }
    else
    {
        rand_value = g_rand_double(rl->random);
        
        if (rand_value > (1.0 - EXPLORATION_THRESHOLD))
        {
            /* Random action */
            action = g_rand_int_range(rl->random, 0, rl->num_actions);
        }
        else
        {
            action = best_action(rl, state);
        }
    }
    
    return action;
} 

void output_action_values (RLearner *rl)
{
	int state  = 0;
	int action = 0;
	
    if (rl == NULL)
    {
        fprintf(stderr, "RLearner-WARNING **: RLearner *rl is NULL\n");
    }
    else
    {
        for(state = 0; state < rl->num_states; state++)
        {
            printf("\n");
            
            for (action = 0; action < rl->num_actions; action++)
            {
                printf("R(%d,%d) = %05f ", state, action, rl->q_values[state][action]);
            }
        }
        
        printf("\n");
    }
}

void output_prefered_state_values (RLearner *rl)
{
	int state  = 0;
	int action = 0;
	
    if (rl == NULL)
    {
        fprintf(stderr, "RLearner-WARNING **: RLearner *rl is NULL\n");
    }
    else
    {
        for(state = 0; state < rl->num_states; state++)
        {
            printf("\n");
            
            action = best_action (rl, state);
            printf("R(%d,%d) = %05f ", state, action, rl->q_values[state][action]);
        }
        
        printf("\n");
    }
}  

void run_trials (RLearner *rl, const unsigned int run)
{
	int    index      = 0;
	int    prev_state = 0;
	int    next_state = 0;
	int    action     = 0;
    double reward     = 0.0;
    
    if (rl == NULL)
    {
        fprintf(stderr, "RLearner-WARNING **: RLearner *rl is NULL\n");
    }
    else
    {
        init_qvalues(rl);

        /* Start State */
        prev_state = 0;
        
        for (index = 0; index <= run; index++)
        {
            action     = semi_uniform(rl, prev_state);
            next_state = rl->next_state(prev_state, action);
            reward     = rl->reward(prev_state, next_state, action);
            
            update_qvalues(rl, prev_state, next_state, action, reward);
            prev_state = next_state;
        }
    }
}
