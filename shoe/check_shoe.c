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

#include "shoe.h"

#include <stdio.h>
#include <stdlib.h>
#include <check.h>

START_TEST(test_create_shoe)
{
    Shoe  *shoe;
    
    shoe = create_shoe(5);
    
    if (shoe == NULL)
    {
        ck_abort_msg ("Failed to create Shoe\n");
    }
    else
    {
        ck_assert_msg( shoe->array   != NULL, "Array is NULL\n");
        ck_assert_msg( shoe->random  != NULL, "Random is NULL\n");
        
        destroy_shoe( shoe );
    }
    
}
END_TEST

START_TEST(test_shuffle_shoe)
{
    Shoe  *shoe;
    gint   card;
    gint   index;
    gint   num_decks = 5;
    
    shoe = create_shoe(num_decks);
    
    if (shoe == NULL)
    {
        ck_abort_msg ("Failed to create Shoe\n");
    }
    else
    {
        shuffle_shoe( shoe );
        
        for (index = 0; index < (num_decks * NUM_CARDS_PER_DECK); index++)
        {
            card = get_card(shoe);
            printf("%d ", card);
        }
        printf("\n");
        
        ck_assert_int_eq( get_card(shoe), -1 );
        
        destroy_shoe( shoe );
    }
    
}
END_TEST

START_TEST(test_get_card_auto_reshuffle)
{
    Shoe  *shoe;
    gint   card;
    gint   index;
    gint   num_decks = 5;
    
    shoe = create_shoe(num_decks);
    
    if (shoe == NULL)
    {
        ck_abort_msg ("Failed to create Shoe\n");
    }
    else
    {
        shuffle_shoe( shoe );
        
        for (index = 0; index < (num_decks * NUM_CARDS_PER_DECK) * 3; index++)
        {
            card = get_card_auto_reshuffle(shoe);
            ck_assert_msg( card != CARD_SHUFFLE_REQUIRED, "Shuffle was requested\n");
            printf("%d ", card);
        }
        printf("\n");
        
        destroy_shoe( shoe );
    }
    
}
END_TEST

START_TEST(test_destroy_shoe)
{
    Shoe  *shoe;
    
    shoe = create_shoe(5);
    
    if (shoe == NULL)
    {
        ck_abort_msg ("Failed to create Shoe\n");
    }
    else
    {
        ck_assert_msg( shoe->array   != NULL, "Array is NULL\n");
        ck_assert_msg( shoe->random  != NULL, "Random is NULL\n");
        ck_assert_int_eq( shoe->current, 0 );
        
        destroy_shoe( shoe );
    }
    
}
END_TEST

START_TEST(test_get_card)
{
    Shoe  *shoe      = NULL;
    gint   num_decks = 5;
    gint   index;
    gint   card;
    
    shoe = create_shoe(num_decks);
    
    if (shoe == NULL)
    {
        ck_abort_msg ("Failed to create Shoe\n");
    }
    else
    {
        for (index = 0; index < (num_decks * NUM_CARDS_PER_DECK); index++)
        {
            card = get_card(shoe);
            printf("%d ", card);
        }
        printf("\n");
        
        ck_assert_int_eq( get_card(shoe), -1 );
        
        destroy_shoe( shoe );
    }
    
}
END_TEST

void shoe_notification (void)
{
    printf("A Shuffle has occurred!\n");
}

START_TEST(test_get_shuffle_notify_callback)
{
    Shoe  *shoe;
    gint   card;
    gint   index;
    gint   num_decks = 5;
    
    shoe = create_shoe(num_decks);
    
    if (shoe == NULL)
    {
        ck_abort_msg ("Failed to create Shoe\n");
    }
    else
    {
        shoe->shuffle_notify_callback = shoe_notification;
        shuffle_shoe( shoe );
        
        for (index = 0; index < (num_decks * NUM_CARDS_PER_DECK) * 3; index++)
        {
            card = get_card_auto_reshuffle(shoe);
            ck_assert_msg( card != CARD_SHUFFLE_REQUIRED, "Shuffle was requested\n");
            printf("%d ", card);
        }
        printf("\n");
        
        destroy_shoe( shoe );
    }
    
}
END_TEST

Suite * atree_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Shoe");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test( tc_core, test_create_shoe );
    tcase_add_test( tc_core, test_shuffle_shoe );
    tcase_add_test( tc_core, test_get_card_auto_reshuffle );
    tcase_add_test( tc_core, test_destroy_shoe );
    tcase_add_test( tc_core, test_get_card );
    tcase_add_test( tc_core, test_get_shuffle_notify_callback );
    
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
