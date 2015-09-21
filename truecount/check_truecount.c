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

#include "truecount.h"

#include <stdio.h>
#include <stdlib.h>
#include <check.h>

START_TEST(test_create_histogram)
{
    Histogram *hist;
    
    hist = create_histogram();
    
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
    
    hist = create_histogram();
    
    if (hist == NULL)
    {
        ck_abort_msg ("Failed to create Histogram\n");
    }
    else
    {   
        for (updates = 1; updates <= 100; updates++)
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

START_TEST(test_get_histogram_count)
{
    Histogram *hist;
    
    hist = create_histogram();
    
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

START_TEST(test_destory_histogram)
{
    Histogram *hist;
    
    hist = create_histogram();
    
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

Suite * atree_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Histogram");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test( tc_core, test_create_histogram );
    tcase_add_test( tc_core, test_update_histogram );
    tcase_add_test( tc_core, test_get_histogram_count );
    tcase_add_test( tc_core, test_destory_histogram );
    
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
