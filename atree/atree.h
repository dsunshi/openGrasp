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

#ifndef _ATREE_H_
#define _ATREE_H_

#include <glib.h>

#define LEAN_RIGHT ((unsigned int)(0.6 * RAND_MAX))

typedef  struct atree_t
{
    GTree *tree;
    GRand *random;
} ATree;

ATree*   a_tree_new   (void);

gint     atree_insert (const ATree *tree, gpointer key, gpointer data);

gpointer atree_random_value (const ATree *tree);
gpointer atree_min_value    (const ATree *tree);
gpointer atree_max_value    (const ATree *tree);

GPtrArray* a_tree_list_all  (ATree *tree);

void     a_tree_destroy     (ATree *tree);

#endif
