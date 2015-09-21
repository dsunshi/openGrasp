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

#include <stdlib.h>
#include <check.h>

static int state_transition_func(const unsigned int prev_state, const unsigned int action)
{
    if (prev_state == 0)
    {
        if (action == 0)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        if (action == 0)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
}

static double reward_func(const unsigned int prev_state, const unsigned int next_state, const unsigned int action)
{
    if ( (prev_state == 1) && (next_state == 1) && (action == 1) )
    {
        return 1.0;
    }
    else
    {
        return 0.0;
    }
}

START_TEST(test_state_transition_func)
{
    ck_assert_int_eq( state_transition_func(0, 0), 0 );
    ck_assert_int_eq( state_transition_func(0, 1), 1 );
    ck_assert_int_eq( state_transition_func(1, 0), 0 );
    ck_assert_int_eq( state_transition_func(1, 1), 1 );
}
END_TEST

START_TEST(test_reward_func)
{
    ck_assert_msg( reward_func(0, 0, 0) == 0.0, NULL );
    ck_assert_msg( reward_func(0, 0, 1) == 0.0, NULL );
    ck_assert_msg( reward_func(0, 1, 0) == 0.0, NULL );
    ck_assert_msg( reward_func(0, 1, 1) == 0.0, NULL );
    ck_assert_msg( reward_func(1, 0, 0) == 0.0, NULL );
    ck_assert_msg( reward_func(1, 0, 1) == 0.0, NULL );
    ck_assert_msg( reward_func(1, 1, 0) == 0.0, NULL );
    ck_assert_msg( reward_func(1, 1, 1) == 1.0, NULL );
}
END_TEST

START_TEST(test_r_learner_new)
{
    RLearner *rl;
    
    const int num_states  = 100;
    const int num_actions = 100;
    
    int state  = 0;
	int action = 0;
    
    rl = r_learner_new(num_states, num_actions);

    if (rl == NULL)
    {
        ck_abort_msg ("Failed to create RLearner\n");
    }
    else
    {
        /* Check initialized values of all structure members */
        ck_assert_int_eq( rl->num_states,  num_states  );
        ck_assert_int_eq( rl->num_actions, num_actions );
        
        for(state = 0; state < rl->num_states; state++)
        {
            for (action = 0; action < rl->num_actions; action++)
            {
                ck_assert_msg( rl->q_values[state][action] == 0.0, NULL );
            }
        }
        
        ck_assert_msg( rl->random     == NULL, NULL );
        ck_assert_msg( rl->next_state == NULL, NULL );
        ck_assert_msg( rl->reward     == NULL, NULL );
        
        r_learner_free( rl );
    }
    
}
END_TEST

START_TEST(test_r_learner_free)
{
    RLearner *rl;
    
    const int num_states  = 100;
    const int num_actions = 100;
    
    rl = r_learner_new(num_states, num_actions);

    if (rl == NULL)
    {
        ck_abort_msg ("Failed to create RLearner\n");
    }
    else
    {
        r_learner_free( rl );
    }
}
END_TEST

START_TEST(test_rleaner_set_state_func)
{
    RLearner *rl;
    
    const int num_states  = 100;
    const int num_actions = 100;
    
    rl = r_learner_new(num_states, num_actions);

    if (rl == NULL)
    {
        ck_abort_msg ("Failed to create RLearner\n");
    }
    else
    {
        rleaner_set_state_func( rl, state_transition_func );
        
        ck_assert_msg( rl->next_state == &state_transition_func, NULL );
        
        r_learner_free( rl );
    }
}
END_TEST

START_TEST(test_rleaner_set_reward_func)
{
    RLearner *rl;
    
    const int num_states  = 100;
    const int num_actions = 100;
    
    rl = r_learner_new(num_states, num_actions);

    if (rl == NULL)
    {
        ck_abort_msg ("Failed to create RLearner\n");
    }
    else
    {
        rleaner_set_reward_func( rl, reward_func );
        
        ck_assert_msg( rl->reward == &reward_func, NULL );
        
        r_learner_free( rl );
    }
}
END_TEST

START_TEST(test_run_trials)
{
    RLearner *rl;
    
    const int num_states  = 2;
    const int num_actions = 2;
    
    rl = r_learner_new(num_states, num_actions);

    if (rl == NULL)
    {
        ck_abort_msg ("Failed to create RLearner\n");
    }
    else
    {
        rleaner_set_reward_func( rl, reward_func );
        rleaner_set_state_func(  rl, state_transition_func );
        g_rand_set_seed(rl->random, 0);
        
        /* Single iteration */
        run_trials( rl, 1 );
        
        ck_assert_msg( rl->q_values[0][0] == 0.0, NULL );
        ck_assert_msg( rl->q_values[0][1] == 0.0, NULL );
        ck_assert_msg( rl->q_values[1][0] == 0.0, NULL );
        ck_assert_msg( rl->q_values[1][1] > 0,    NULL );
        ck_assert_msg( rl->q_values[1][1] < 0.2,  NULL );
        
        /* 500 learning iterations */
        run_trials( rl, 500 );
        
        /* rl->q_values[0][0] should be the smallest value */
        ck_assert_msg( rl->q_values[0][0] < rl->q_values[0][1], NULL );
        ck_assert_msg( rl->q_values[0][0] < rl->q_values[1][0], NULL );
        ck_assert_msg( rl->q_values[0][0] < rl->q_values[1][1], NULL );
        
        /* rl->q_values[0][1] should be the 2nd-largest value */
        ck_assert_msg( rl->q_values[0][1] > rl->q_values[1][0], NULL );
        ck_assert_msg( rl->q_values[0][1] < rl->q_values[1][1], NULL );
        
        /* rl->q_values[1][1] should be the largest value */
        ck_assert_msg( rl->q_values[1][0] < rl->q_values[1][1], NULL );
        
        r_learner_free( rl );
    }
}
END_TEST

Suite * rlearner_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("RLearner");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test( tc_core, test_state_transition_func );
    tcase_add_test( tc_core, test_reward_func );
    tcase_add_test( tc_core, test_r_learner_new );
    tcase_add_test( tc_core, test_r_learner_free );
    tcase_add_test( tc_core, test_rleaner_set_state_func );
    tcase_add_test( tc_core, test_rleaner_set_reward_func );
    tcase_add_test( tc_core, test_run_trials );
    
    suite_add_tcase(s, tc_core);

    return s;
}

int main()
{
    int      number_failed;
    Suite   *s;
    SRunner *sr;
    
    s  = rlearner_suite();
    sr = srunner_create(s);
    
    srunner_run_all(sr, CK_NORMAL);
    
    number_failed = srunner_ntests_failed(sr);
    
    srunner_free(sr);
    
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
