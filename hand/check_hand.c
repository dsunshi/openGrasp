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

#include "hand.h"

#include <stdio.h>
#include <stdlib.h>
#include <check.h>

START_TEST(test_create_hand)
{
    Hand *hand;
    
    hand = create_hand();
    
    if (hand == NULL)
    {
        ck_abort_msg ("Failed to create Hand\n");
    }
    else
    {
        ck_assert_int_eq( hand->size, 0 );
        
        ck_assert_int_eq( hand->cards[0],  0 );
        ck_assert_int_eq( hand->cards[1],  0 );
        ck_assert_int_eq( hand->cards[2],  0 );
        ck_assert_int_eq( hand->cards[3],  0 );
        ck_assert_int_eq( hand->cards[4],  0 );
        ck_assert_int_eq( hand->cards[5],  0 );
        ck_assert_int_eq( hand->cards[6],  0 );
        ck_assert_int_eq( hand->cards[7],  0 );
        ck_assert_int_eq( hand->cards[8],  0 );
        ck_assert_int_eq( hand->cards[9],  0 );
        ck_assert_int_eq( hand->cards[10], 0 );
        ck_assert_int_eq( hand->cards[11], 0 );
        ck_assert_int_eq( hand->cards[12], 0 );
        ck_assert_int_eq( hand->cards[13], 0 );
        ck_assert_int_eq( hand->cards[14], 0 );
        
        destroy_hand( hand );
    }
}
END_TEST

START_TEST(test_receive_card)
{
    Hand *hand;
    gint  index;
    
    hand = create_hand();
    
    if (hand == NULL)
    {
        ck_abort_msg ("Failed to create Hand\n");
    }
    else
    {
    
        for (index = 2; index <= 11; index++)
        {
            receive_card( hand, index );
            ck_assert_int_eq( get_card_value( hand, index - 2 ), index );
        }
        
        ck_assert_int_eq( hand->size, index - 2 );
        
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

    tcase_add_test( tc_core, test_create_hand );
    tcase_add_test( tc_core, test_receive_card );
    
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
