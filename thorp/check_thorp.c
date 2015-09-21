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

#include "thorp.h"

#include <stdio.h>
#include <stdlib.h>
#include <check.h>

START_TEST(test_thorp)
{
    Hand  *hand   = create_hand();
    Action action;
    
    if (hand == NULL)
    {
        ck_abort_msg ("Failed to create Hand\n");
    }
    else
    {
        receive_card( hand, 4 );
        receive_card( hand, 8 );
        action = get_thorp_action( hand, 7 );
        ck_assert_int_eq( action,  HIT );
        
        
        receive_card( hand, 8 );
        action = get_thorp_action( hand, 7 );
        ck_assert_int_eq( action,  STAND );
    
        destroy_hand( hand );
    }
}
END_TEST


Suite * atree_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Hand");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test( tc_core, test_thorp );
    
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
