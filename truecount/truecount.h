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

#ifndef _TRUE_COUNT_H_
#define _TRUE_COUNT_H_

#include <glib.h>

#include "../blackjack/cards.h"

#define KO_COUNT        0
#define HI_LOW_COUNT    1
#define HI_OPT1_COUNT   2
#define HI_OPT2_COUNT   3
#define ZEN_COUNT       4
#define OMEGA_II_COUNT  5
#define NUM_COUNT_TYPE  6

/* Clamp the true count to 3-bits in length */
#define TRUE_COUNT_MIN  1
#define TRUE_COUNT_MAX  7

typedef struct TrueCount_t
{
	gint  *counts;
} TrueCount;

TrueCount *create_true_count(void);
void destroy_true_count(TrueCount *count);

void  reset_true_count  (TrueCount *count);
void  update_true_count (TrueCount *count, gint8 card);
gint  get_true_count    (TrueCount *count, gint8 type);

#endif

