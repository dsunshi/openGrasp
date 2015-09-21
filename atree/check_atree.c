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

#include "atree.h"

#include <stdio.h>
#include <stdlib.h>
#include <check.h>

START_TEST(test_a_tree_new)
{
    ATree *at;
    
    at = a_tree_new();
    
    if (at == NULL)
    {
        ck_abort_msg ("Failed to create ATree\n");
    }
    else
    {
        a_tree_destroy( at );
    }
}
END_TEST

START_TEST(test_atree_insert)
{
    ATree *at;
    gint   result;
    gint   key;
    gint   data;
    
    at = a_tree_new();
    
    if (at == NULL)
    {
        ck_abort_msg ("Failed to create ATree\n");
    }
    else
    {
        key  = 1;
        data = 1;
        
        result = atree_insert( at, GINT_TO_POINTER(&key), GINT_TO_POINTER(&data) );
        ck_assert_int_eq( result, 0 );
        
        a_tree_destroy( at );
    }
}
END_TEST

START_TEST(test_atree_random_value)
{
    ATree *at;
    gint  result;
    gint  key;
    gint  data;
    gint  *random;
    
    at = a_tree_new();
    
    if (at == NULL)
    {
        ck_abort_msg ("Failed to create ATree\n");
    }
    else
    {
        key  = 1;
        data = 1;
        
        g_rand_set_seed( at->random, 0 );
        
        result = atree_insert( at, GINT_TO_POINTER(&key), GINT_TO_POINTER(&data) );
        ck_assert_int_eq( result, 0 );
		
		random = atree_random_value( at );
        
        if (random == NULL)
        {
            ck_abort_msg ("random values is NULL\n");
        }
        else
        {
            ck_assert_int_eq( *random, data );
        }
        
        a_tree_destroy( at );
    }
}
END_TEST

START_TEST(test_atree_min_value)
{
    ATree *at;
    gint   result;
    gint   key_a;
    gint   data_a;
    gint   key_b;
    gint   data_b;
    gint  *min;
    GRand *random;
    
    at = a_tree_new();
    
    if (at == NULL)
    {
        ck_abort_msg ("Failed to create ATree\n");
    }
    else
    {
        random = g_rand_new_with_seed( (guint32) at );
        key_a  = g_rand_int( random );
        data_a = g_rand_int( random );
        key_b  = g_rand_int( random );
        data_b = g_rand_int( random );
        
        result = atree_insert( at, GINT_TO_POINTER(&key_a), GINT_TO_POINTER(&data_a) );
        ck_assert_int_eq( result, 0 );
        
        result = atree_insert( at, GINT_TO_POINTER(&key_b), GINT_TO_POINTER(&data_b) );
        ck_assert_int_eq( result, 0 );
        
        min    = atree_min_value( at );
        
        if (min == NULL)
        {
            ck_abort_msg ("maximum values is NULL\n");
        }
        else
        {   
            if (key_a < key_b)
            {
                ck_assert_int_eq( *min, data_a );
            }
            else
            {
                ck_assert_int_eq( *min, data_b );
            }
        }
        
        a_tree_destroy( at );
    }
}
END_TEST

START_TEST(test_atree_max_value)
{
    ATree *at;
    gint   result;
    gint   key_a;
    gint   data_a;
    gint   key_b;
    gint   data_b;
    gint  *max;
    GRand *random;
    
    at = a_tree_new();
    
    if (at == NULL)
    {
        ck_abort_msg ("Failed to create ATree\n");
    }
    else
    {
        random = g_rand_new_with_seed( (guint32) at );
        key_a  = g_rand_int( random );
        data_a = g_rand_int( random );
        key_b  = g_rand_int( random );
        data_b = g_rand_int( random );
        
        result = atree_insert( at, GINT_TO_POINTER(&key_a), GINT_TO_POINTER(&data_a) );
        ck_assert_int_eq( result, 0 );
        
        result = atree_insert( at, GINT_TO_POINTER(&key_b), GINT_TO_POINTER(&data_b) );
        ck_assert_int_eq( result, 0 );
        
        max    = atree_max_value( at );
        
        if (max == NULL)
        {
            ck_abort_msg ("maximum values is NULL\n");
        }
        else
        {   
            if (key_a > key_b)
            {
                ck_assert_int_eq( *max, data_a );
            }
            else
            {
                ck_assert_int_eq( *max, data_b );
            }
        }
        
        a_tree_destroy( at );
    }
}
END_TEST

START_TEST(test_atree_minmax)
{
    ATree *at;
    gint   result;
    gint   *key;
    gint   *data;
    gint   limit;
    gint   index;
    gint   min;
    gint   max;
    gint   *min_from_tree;
    gint   *max_from_tree;
    GRand  *random;
    
    at = a_tree_new();
    
    if (at == NULL)
    {
        ck_abort_msg ("Failed to create ATree\n");
    }
    else
    {
        limit  = 100;
        random = g_rand_new_with_seed( (guint32) at );
        min    = 0;
        max    = 0;
         
        for (index = 0; index < limit; index++)
        {
            key  = g_new(gint, 1);
            data = g_new(gint, 1);
            
            *key  = g_rand_int( random );
            *data = *key;
            
            if (*key >= max)
            {
                max = *data;
            }
            else
            {
                if (*key <= min)
                {
                    min = *data;
                }
                else
                {
                    /* Do nothing */
                }
            }
            
            result = atree_insert( at, GINT_TO_POINTER(key), GINT_TO_POINTER(data) );
            ck_assert_int_eq( result, 0 );
        }
        
        max_from_tree = atree_max_value( at );
        min_from_tree = atree_min_value( at );
        
        ck_assert_int_eq( *max_from_tree, max );
        ck_assert_int_eq( *min_from_tree, min );
        
        a_tree_destroy( at );
    }
}
END_TEST

static void my_printf (gpointer data, gpointer user_data)
{
    printf(user_data, *((gint *)data) );
}

START_TEST(test_a_tree_list_all)
{
    ATree *at;
    gint  result;
    gint  key;
    gint  data;
    gint  *random;
    GPtrArray *list;
    gint data2;
    
    at = a_tree_new();
    
    if (at == NULL)
    {
        ck_abort_msg ("Failed to create ATree\n");
    }
    else
    {
        key   = 1;
        data  = 1;
        data2 = 2;
        
        g_rand_set_seed( at->random, 0 );
        
        result = atree_insert( at, GINT_TO_POINTER(&key), GINT_TO_POINTER(&data) );
        ck_assert_int_eq( result, 0 );
		
		random = atree_random_value( at );
        
        if (random == NULL)
        {
            ck_abort_msg ("random values is NULL\n");
        }
        else
        {
            ck_assert_int_eq( *random, data );
        }
        
        (void)atree_insert( at, GINT_TO_POINTER(&key), GINT_TO_POINTER(&data2) );
        
        list = a_tree_list_all( at );
        g_ptr_array_foreach( list, (GFunc) my_printf, "%d \n" );
        
        ck_assert_int_eq( *((int *)g_ptr_array_index( list, 0)), 1 );
        ck_assert_int_eq( *((int *)g_ptr_array_index( list, 1)), 2 );
        
        a_tree_destroy( at );
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

    tcase_add_test( tc_core, test_a_tree_new );
    tcase_add_test( tc_core, test_atree_insert );
    tcase_add_test( tc_core, test_atree_random_value );
    tcase_add_test( tc_core, test_atree_min_value );
    tcase_add_test( tc_core, test_atree_max_value );
    /*tcase_add_test( tc_core, test_atree_minmax );*/
    tcase_add_test( tc_core, test_a_tree_list_all );
    
    
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
