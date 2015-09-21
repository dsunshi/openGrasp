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

#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

#include "atree.h"

#define TREE_RIGHT (1)
#define TREE_LEFT  (-1)
#define TREE_MATCH (0)

static void a_tree_list_all_helper (gpointer key,  gpointer value, gpointer data);
static void g_ptr_array_list_all_helper (gpointer data, gpointer user_data);

static gint g_int_compare_func    (gconstpointer a, gconstpointer b);
static gint max_value_searcher    (gpointer key,    gpointer user_data);
static gint min_value_searcher    (gpointer key,    gpointer user_data);
static gint random_value_searcher (gpointer key,    gpointer user_data);
static gint tree_root_searcher    (gpointer key,    gpointer user_data);
static void key_destroy           (gpointer data);
static void value_destroy         (gpointer data);

#define ATREE_TO_GTREE(a)   ( (GTree *)((a)->tree) )

#ifndef UNUSED_ARGUMENT
  #define UNUSED_ARGUMENT(arg) if ((arg) != 0) {}
#endif

static void g_ptr_array_list_all_helper (gpointer data, gpointer user_data)
{
    GPtrArray *list  = (GPtrArray *) user_data;
    
    g_ptr_array_add( list, data );
}

static void a_tree_list_all_helper (gpointer key, gpointer value, gpointer data)
{
	GPtrArray *array = (GPtrArray *) value;
    
    g_ptr_array_foreach( array, g_ptr_array_list_all_helper, data );
}

GPtrArray * a_tree_list_all (ATree *tree)
{
	GPtrArray *list = g_ptr_array_new();
	
	g_tree_foreach( ATREE_TO_GTREE( tree ), (GTraverseFunc) a_tree_list_all_helper, list );
	
	return list;
}

static void key_destroy (gpointer data)
{
	UNUSED_ARGUMENT(data);
}

static void value_destroy (gpointer data)
{
    g_ptr_array_free( (GPtrArray *)data, TRUE);
}

static gint g_int_compare_func (gconstpointer a, gconstpointer b)
{
    const int int_a = GPOINTER_TO_INT (a);
    const int int_b = GPOINTER_TO_INT (b);

    return (int_a - int_b);
}

static gint tree_root_searcher (gpointer key,    gpointer user_data)
{
	UNUSED_ARGUMENT(key);
	UNUSED_ARGUMENT(user_data);
	
    return TREE_MATCH;
}

static gint max_value_searcher (gpointer key, gpointer user_data)
{
	gint tree_direction = TREE_MATCH;
	gint *depth         = (gint *)user_data;
	
	UNUSED_ARGUMENT(key);
	
	if (*depth == 0)
	{
		tree_direction = TREE_MATCH;
	}
	else
	{
		tree_direction = TREE_RIGHT;
		*depth = *depth - 1;
	}
	
	return tree_direction;
}

static gint min_value_searcher (gpointer key, gpointer user_data)
{
	gint tree_direction = TREE_MATCH;
	gint *depth         = (gint *)user_data;
	
	UNUSED_ARGUMENT(key);
	
	if (*depth == 0)
	{
		tree_direction = TREE_MATCH;
	}
	else
	{
		tree_direction = TREE_LEFT;
		*depth = *depth - 1;
	}
	
	return tree_direction;
}

static gint random_value_searcher (gpointer key, gpointer user_data)
{
	gint tree_direction = TREE_MATCH;
	gint *depth         = (gint *)user_data;
	
	UNUSED_ARGUMENT(key);
	
	if (*depth == 0)
	{
		tree_direction = TREE_MATCH;
	}
	else
	{
		if (rand() < LEAN_RIGHT)
		{
			tree_direction = TREE_RIGHT;
		}
		else
		{
			tree_direction = TREE_LEFT;
		}
		
		*depth = *depth - 1;
	}
	
	return tree_direction;
}

gpointer atree_max_value (const ATree *tree)
{
	gpointer max_value = NULL;
	gint     depth     = 0;
	
	if (tree == NULL)
    {
        g_warning("ATree-WARNING **: ATree is null - returning NULL\n");
    }
    else
    {
		depth     = g_tree_height( ATREE_TO_GTREE(tree) ) - 1;
        max_value = g_tree_search( ATREE_TO_GTREE(tree), (GCompareFunc)max_value_searcher, &depth);
		
		if ( (max_value == NULL) && (depth == 0) )
		{
			max_value = g_tree_search( ATREE_TO_GTREE(tree), (GCompareFunc)tree_root_searcher, NULL);
            max_value = g_ptr_array_index((GPtrArray *) max_value, 0);
		}
		else
		{
			max_value = g_ptr_array_index((GPtrArray *) max_value, 0);
		}
    }
	
	return max_value;
}

gpointer atree_min_value (const ATree *tree)
{
	gpointer min_value = NULL;
	gint     depth     = 0;
	
	if (tree == NULL)
    {
        g_warning("ATree-WARNING **: ATree is null - returning NULL\n");
    }
    else
    {
		depth     = g_tree_height( ATREE_TO_GTREE(tree) ) - 1;
        min_value = g_tree_search( ATREE_TO_GTREE(tree), (GCompareFunc)min_value_searcher, &depth);
		
		if ( (min_value == NULL) && (depth == 0) )
		{
			min_value = g_tree_search( ATREE_TO_GTREE(tree), (GCompareFunc)tree_root_searcher, NULL);
            min_value = g_ptr_array_index((GPtrArray *) min_value, 0);
		}
		else
		{
			min_value = g_ptr_array_index((GPtrArray *) min_value, 0);
		}
    }
	
	return min_value;
}

gpointer atree_random_value (const ATree *tree)
{
	gpointer   random_value = NULL;
	gint       depth        = 0;
    GPtrArray *array        = NULL;
	
	if (tree == NULL)
    {
        g_warning("ATree-WARNING **: ATree is null - returning NULL\n");
    }
    else
    {
        depth        = g_rand_int_range(tree->random, 0, g_tree_height( ATREE_TO_GTREE(tree) ));
		random_value = g_tree_search( ATREE_TO_GTREE(tree) , (GCompareFunc)random_value_searcher, &depth);
        
        array        = (GPtrArray *) random_value;
		random_value = g_ptr_array_index( array, g_rand_int_range(tree->random, 0, array->len) );
    }
	
	return random_value;
}

gint atree_insert (const ATree *tree, gpointer key, gpointer data)
{
	gboolean   key_found = FALSE;
	gpointer   orig_key  = NULL;
	gpointer   value     = NULL;
	GPtrArray *array     = NULL;
	
	key_found = g_tree_lookup_extended( ATREE_TO_GTREE(tree) , key, &orig_key, &value);
	
	if (key_found != FALSE)
	{
		/* Pointer array already exists with this key */
		array = (GPtrArray *) value;
		g_ptr_array_add( array, data );
	}
	else
	{
		/* This key does not exist in the tree (yet) */
		array = g_ptr_array_new();
        
        if (array == NULL)
        {
            g_error("ATree-CRITICAL **: Could not allocate memory for ATree\n");
        }
        else
        {
            g_ptr_array_add( array, data );
            g_tree_insert(   ATREE_TO_GTREE(tree), key, array);
        }
	}
	
	return 0;
}

ATree *a_tree_new (void)
{
    ATree *tree = NULL;
    
    tree = (ATree *) g_new(ATree, 1);
    
    if (tree == NULL)
    {
        g_error("ATree-CRITICAL **: Could not allocate memory for ATree\n");
    }
    else
    {
        tree->tree   = g_tree_new_full( (GCompareDataFunc)g_int_compare_func,
                                        NULL,
                                        (GDestroyNotify)key_destroy,
                                        (GDestroyNotify)value_destroy);
		
        tree->random = g_rand_new();
    }
    
	return tree;
}

void a_tree_destroy (ATree *tree)
{
    g_tree_destroy( ATREE_TO_GTREE(tree) );
    g_free(tree);
}
