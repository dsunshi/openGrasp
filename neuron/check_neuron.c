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

#include "neuron.h"

#include <stdio.h>
#include <stdlib.h>
#include <check.h>

START_TEST(test_create_neuron)
{
    Neuron *n;
    
    n = create_neuron(10);
    
    if (n == NULL)
    {
        ck_abort_msg ("Failed to create Neuron\n");
    }
    else
    {
        destory_neuron( n );
    }
}
END_TEST

START_TEST(test_calc_neuron_output)
{
    Neuron *n;
    
    n = create_neuron(2);
    
    if (n == NULL)
    {
        ck_abort_msg ("Failed to create Neuron\n");
    }
    else
    {
        n->value->x[0] = 0;
        n->value->x[1] = 0;
        
        n->value->w[0] = 0;
        n->value->w[1] = 0;
        
        n->value->threshold = 1;
        
        calc_neuron_output( n );
        
        ck_assert_int_eq( n->output, NEURON_OUTPUT_LOW);
        
        n->value->x[0] = 0;
        n->value->x[1] = 2;
        
        n->value->w[0] = 0;
        n->value->w[1] = 1;
        
        n->value->threshold = 1;
        
        calc_neuron_output( n );
        
        ck_assert_int_eq( n->output, NEURON_OUTPUT_HIGH);
    
        destory_neuron( n );
    }
}
END_TEST

Suite * atree_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("ATree");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test( tc_core, test_create_neuron );
    tcase_add_test( tc_core, test_calc_neuron_output );
    
    
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
