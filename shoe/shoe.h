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

#ifndef _SHOE_H_
#define _SHOE_H_

#include <glib.h>

#include "../blackjack/cards.h"

#define NUM_VALUES_PER_SUIT		NUM_CARDS
#define NUM_SUITS_PER_DECK		NUM_SUITS
#define NUM_TEN_VAL_PER_SUIT	((gint8) 4 )

#define CARD_SHUFFLE_REQUIRED   (-1)

typedef  struct shoe_t
{
    GArray *array;
    GRand  *random;
    gint    current;
    gint    num_cards;
    void  (*shuffle_notify_callback) (void);
} Shoe;

Shoe  *create_shoe  ( const unsigned int num_decks );
void   shuffle_shoe ( Shoe *shoe );
void   destroy_shoe ( Shoe *shoe );

gint8  get_card ( Shoe *shoe );
gint8  get_card_auto_reshuffle ( Shoe *shoe );

#define SHOE_SET_SHUFFLE_CALLBACK(s,c) ((s)->shuffle_notify_callback = (c))
#define SHOE_GET_REMAINING_CARDS(s)    ((gint) (((s)->num_cards) - ((s)->current)))

#endif