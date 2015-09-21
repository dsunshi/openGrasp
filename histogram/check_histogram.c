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

#include "histogram.h"

#include <stdio.h>
#include <stdlib.h>
#include <check.h>

static void observe_card (Histogram *hist, gint card, gint count)
{
	gint index;
	
	/* Observe card 'card', 'count' # of times (with verify) */
	for (index = 0; index < count; index++)
	{
		update_histogram(hist, card);
	}
	
	ck_assert_int_eq( get_histogram_count(hist, card), count );
}

START_TEST(test_create_histogram)
{
    Histogram *hist;
    
    hist = create_histogram(1);
    
    if (hist == NULL)
    {
        ck_abort_msg ("Failed to create Histogram\n");
    }
    else
    {   
        destory_histogram( hist );
    }
}
END_TEST

START_TEST(test_update_histogram)
{
    Histogram *hist;
    gint index;
    gint updates;
    gint num_decks = 5;
    
    hist = create_histogram(num_decks);
    
    if (hist == NULL)
    {
        ck_abort_msg ("Failed to create Histogram\n");
    }
    else
    {   
        for (updates = 1; updates <= num_decks * 4; updates++)
        {
            for (index = 2; index <= 11; index++)
            {
                update_histogram(hist, index);
            }
            
            for (index = 2; index <= 11; index++)
            {
				ck_assert_int_eq( get_histogram_count(hist, index), updates );
            }
        }
        
        destory_histogram( hist );
    }
}
END_TEST

START_TEST(test_get_bust_odds)
{
    Histogram *hist;
    gint num_decks = 1;
    gint j;
    
    hist = create_histogram(num_decks);
    
    if (hist == NULL)
    {
        ck_abort_msg ("Failed to create Histogram\n");
    }
    else
    {
		/* Bust Case */
        ck_assert_int_eq( get_bust_odds(hist, BUST), PERCENT );
        
        /* Every card, but 10's has been seen */
		for (j = 2; j < 10; j++)
		{
			observe_card(hist, j, 4);
		}
		observe_card(hist, ACE, 4);
        
        ck_assert_int_eq( get_bust_odds(hist, 20), PERCENT );
        ck_assert_int_eq( get_bust_odds(hist, 19), PERCENT );
        ck_assert_int_eq( get_bust_odds(hist, 18), PERCENT );
        ck_assert_int_eq( get_bust_odds(hist, 17), PERCENT );
        ck_assert_int_eq( get_bust_odds(hist, 16), PERCENT );
        ck_assert_int_eq( get_bust_odds(hist, 15), PERCENT );
        ck_assert_int_eq( get_bust_odds(hist, 14), PERCENT );
        ck_assert_int_eq( get_bust_odds(hist, 13), PERCENT );
        ck_assert_int_eq( get_bust_odds(hist, 12), PERCENT );
        ck_assert_int_eq( get_bust_odds(hist, 11), 0 );
        ck_assert_int_eq( get_bust_odds(hist, 10), 0 );
        ck_assert_int_eq( get_bust_odds(hist,  9), 0 );
        ck_assert_int_eq( get_bust_odds(hist,  8), 0 );
        ck_assert_int_eq( get_bust_odds(hist,  7), 0 );
        ck_assert_int_eq( get_bust_odds(hist,  6), 0 );
        ck_assert_int_eq( get_bust_odds(hist,  5), 0 );
        ck_assert_int_eq( get_bust_odds(hist,  4), 0 );
        ck_assert_int_eq( get_bust_odds(hist,  3), 0 );
        ck_assert_int_eq( get_bust_odds(hist,  2), 0 );
        
        reset_histogram( hist );
		
		/* Every card, but 10's and ACE's has been seen */
		for (j = 2; j < 10; j++)
		{
			observe_card(hist, j, 4);
		}
        
        ck_assert_int_eq( get_bust_odds(hist, 20), ((gint)(0.8 * PERCENT)) );
        ck_assert_int_eq( get_bust_odds(hist, 19), ((gint)(0.8 * PERCENT)) );
        ck_assert_int_eq( get_bust_odds(hist, 18), ((gint)(0.8 * PERCENT)) );
        ck_assert_int_eq( get_bust_odds(hist, 17), ((gint)(0.8 * PERCENT)) );
        ck_assert_int_eq( get_bust_odds(hist, 16), ((gint)(0.8 * PERCENT)) );
        ck_assert_int_eq( get_bust_odds(hist, 15), ((gint)(0.8 * PERCENT)) );
        ck_assert_int_eq( get_bust_odds(hist, 14), ((gint)(0.8 * PERCENT)) );
        ck_assert_int_eq( get_bust_odds(hist, 13), ((gint)(0.8 * PERCENT)) );
        ck_assert_int_eq( get_bust_odds(hist, 12), ((gint)(0.8 * PERCENT)) );
        ck_assert_int_eq( get_bust_odds(hist, 11), 0 );
        ck_assert_int_eq( get_bust_odds(hist, 10), 0 );
        ck_assert_int_eq( get_bust_odds(hist,  9), 0 );
        ck_assert_int_eq( get_bust_odds(hist,  8), 0 );
        ck_assert_int_eq( get_bust_odds(hist,  7), 0 );
        ck_assert_int_eq( get_bust_odds(hist,  6), 0 );
        ck_assert_int_eq( get_bust_odds(hist,  5), 0 );
        ck_assert_int_eq( get_bust_odds(hist,  4), 0 );
        ck_assert_int_eq( get_bust_odds(hist,  3), 0 );
        ck_assert_int_eq( get_bust_odds(hist,  2), 0 );
		
		reset_histogram( hist );
		
		/* Every card, but 9/10's and ACE's has been seen */
		for (j = 2; j < 9; j++)
		{
			observe_card(hist, j, 4);
		}
        
        ck_assert_int_eq( get_bust_odds(hist, 20), ((gint)(0.83 * PERCENT)) );
        ck_assert_int_eq( get_bust_odds(hist, 19), ((gint)(0.83 * PERCENT)) );
        ck_assert_int_eq( get_bust_odds(hist, 18), ((gint)(0.83 * PERCENT)) );
        ck_assert_int_eq( get_bust_odds(hist, 17), ((gint)(0.83 * PERCENT)) );
        ck_assert_int_eq( get_bust_odds(hist, 16), ((gint)(0.83 * PERCENT)) );
        ck_assert_int_eq( get_bust_odds(hist, 15), ((gint)(0.83 * PERCENT)) );
        ck_assert_int_eq( get_bust_odds(hist, 14), ((gint)(0.83 * PERCENT)) );
        ck_assert_int_eq( get_bust_odds(hist, 13), ((gint)(0.83 * PERCENT)) );
        ck_assert_int_eq( get_bust_odds(hist, 12), ((gint)(0.666 * PERCENT)) );
        ck_assert_int_eq( get_bust_odds(hist, 11), 0 );
        ck_assert_int_eq( get_bust_odds(hist, 10), 0 );
        ck_assert_int_eq( get_bust_odds(hist,  9), 0 );
        ck_assert_int_eq( get_bust_odds(hist,  8), 0 );
        ck_assert_int_eq( get_bust_odds(hist,  7), 0 );
        ck_assert_int_eq( get_bust_odds(hist,  6), 0 );
        ck_assert_int_eq( get_bust_odds(hist,  5), 0 );
        ck_assert_int_eq( get_bust_odds(hist,  4), 0 );
        ck_assert_int_eq( get_bust_odds(hist,  3), 0 );
        ck_assert_int_eq( get_bust_odds(hist,  2), 0 );
		
		reset_histogram( hist );
		
		/* Every card, but 8/9/10's and ACE's has been seen */
		for (j = 2; j < 8; j++)
		{
			observe_card(hist, j, 4);
		}
		
		/* ----- > 13
		   4 + 4 + 16 = 24 will bust (8, 9, 10)
		   4 will not (ACE)
		   ----- = 13
		   4 + 16 = 20 will bust (9, 10)
		   8 will not bust (8, ACE)
		   ----- < 13
		   16 will bust (10)
		   12 will not bust (8, 9, ACE)
		*/
        
        ck_assert_int_eq( get_bust_odds(hist, 20), ((gint)(0.857 * PERCENT)) );
        ck_assert_int_eq( get_bust_odds(hist, 19), ((gint)(0.857 * PERCENT)) );
        ck_assert_int_eq( get_bust_odds(hist, 18), ((gint)(0.857 * PERCENT)) );
        ck_assert_int_eq( get_bust_odds(hist, 17), ((gint)(0.857 * PERCENT)) );
        ck_assert_int_eq( get_bust_odds(hist, 16), ((gint)(0.857 * PERCENT)) );
        ck_assert_int_eq( get_bust_odds(hist, 15), ((gint)(0.857 * PERCENT)) );
        ck_assert_int_eq( get_bust_odds(hist, 14), ((gint)(0.857 * PERCENT)) );
        ck_assert_int_eq( get_bust_odds(hist, 13), ((gint)(0.714 * PERCENT)) );
        ck_assert_int_eq( get_bust_odds(hist, 12), ((gint)(0.571 * PERCENT)) );
        ck_assert_int_eq( get_bust_odds(hist, 11), 0 );
        ck_assert_int_eq( get_bust_odds(hist, 10), 0 );
        ck_assert_int_eq( get_bust_odds(hist,  9), 0 );
        ck_assert_int_eq( get_bust_odds(hist,  8), 0 );
        ck_assert_int_eq( get_bust_odds(hist,  7), 0 );
        ck_assert_int_eq( get_bust_odds(hist,  6), 0 );
        ck_assert_int_eq( get_bust_odds(hist,  5), 0 );
        ck_assert_int_eq( get_bust_odds(hist,  4), 0 );
        ck_assert_int_eq( get_bust_odds(hist,  3), 0 );
        ck_assert_int_eq( get_bust_odds(hist,  2), 0 );
        
        destory_histogram( hist );
    }
}
END_TEST

START_TEST(test_destory_histogram)
{
    Histogram *hist;
    
    hist = create_histogram(10);
    
    if (hist == NULL)
    {
        ck_abort_msg ("Failed to create Histogram\n");
    }
    else
    {   
        destory_histogram( hist );
    }
}
END_TEST

START_TEST(test_reset_histogram)
{
    Histogram *hist;
    gint index, j;
    
    hist = create_histogram(1);
    
    if (hist == NULL)
    {
        ck_abort_msg ("Failed to create Histogram\n");
    }
    else
    {
        for (index = 0; index < 4; index++)
        {
            for (j = 2; j < 10; j++)
            {
                update_histogram(hist, j);
            }
            
            update_histogram(hist, ACE);
        }
        
        reset_histogram( hist );
        
        for (index = 0; index < 4; index++)
        {
            for (j = 2; j < 10; j++)
            {
                ck_assert_int_eq( get_histogram_count(hist,  j), 0 );
            }
            
            ck_assert_int_eq( get_histogram_count(hist,  ACE), 0 );
        }
        
        destory_histogram( hist );
    }
}
END_TEST

Suite * atree_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Histogram");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test( tc_core, test_create_histogram );
    tcase_add_test( tc_core, test_update_histogram );
    tcase_add_test( tc_core, test_destory_histogram );
    tcase_add_test( tc_core, test_reset_histogram );
    tcase_add_test( tc_core, test_get_bust_odds );
    
    suite_add_tcase(s, tc_core);

    return s;
}

int main()
{
    int      number_failed;
    Suite   *s;
    SRunner *sr;
    
    s  = atree_suite();
    sr = srunner_create(s);
    
    srunner_run_all(sr, CK_NORMAL);
    
    number_failed = srunner_ntests_failed(sr);
    
    srunner_free(sr);
    
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
