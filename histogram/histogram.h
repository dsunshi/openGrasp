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

#ifndef _HISTOGRAM_H_
#define _HISTOGRAM_H_

#include <glib.h>

#include "../blackjack/cards.h"

#define PERCENT 127
#define BUST    22

typedef  struct histogram_t
{
    gint *histogram;
    gint  num_decks;
} Histogram;

Histogram  *create_histogram( gint num_decks );

void update_histogram    ( Histogram *hist, gint8 card );
gint get_histogram_count ( Histogram *hist, gint8 card );
gint get_bust_odds       ( Histogram *hist, gint8 value );

void reset_histogram    ( Histogram *hist );
void destory_histogram  ( Histogram *hist );

#endif