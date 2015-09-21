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

#ifndef _RLEARNER_H_
#define _RLEARNER_H_

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef EXPLORATION_THRESHOLD
  #define EXPLORATION_THRESHOLD 0.75
#endif

#ifndef BETA
  #define BETA 0.1
#endif

#ifndef GAMMA
  #define GAMMA 0.7
#endif

typedef  struct rlearner_t
{
    unsigned int num_states;
    unsigned int num_actions;
    
    double ** q_values;
    
    GRand *random;
    
    /* Arguments:         state,              action              */
    int    (*next_state) (const unsigned int, const unsigned int);
    /* Arguments:         previous state,     next state,         action              */
    double (*reward)     (const unsigned int, const unsigned int, const unsigned int);
} RLearner;

RLearner *r_learner_new (const unsigned int num_states, const unsigned int num_actions);

void r_learner_free (RLearner *rl);

void run_trials (RLearner *rl, const unsigned int run);

void output_action_values (RLearner *rl);
void output_prefered_state_values (RLearner *rl);

#define rleaner_set_state_func(rl, f) (rl)->next_state = (&(f))
#define rleaner_set_reward_func(rl, f) (rl)->reward = (&(f))

#endif
