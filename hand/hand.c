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

Hand *create_hand (void)
{
    Hand *h = NULL;
    
    h = g_new(Hand, 1);
    
    if ( h == NULL )
    {
        g_error("Hand-CRITICAL **: Could not allocate memory for Hand\n");
    }
    else
    {
        h->cards = g_new(guint8, MAX_HAND_SIZE);
        
        if ( h->cards == NULL )
        {
            g_error("Hand-CRITICAL **: Could not allocate memory for Hand\n");
        }
        else
        {
            reset_hand( h );
        }
    }
    
    return h;
}

void  destroy_hand (Hand *h)
{
    g_free( h->cards );
    g_free( h );
}

guint8 get_card_value (Hand *h, gint card)
{
    g_assert( card >= 0 );
    g_assert( card < h->size );
    
    return h->cards[card];
}

void receive_card (Hand *h, gint card)
{
    g_assert( card >= 2  );
    g_assert( card <= 11 );
    
    if ( h->size == (MAX_HAND_SIZE - 1) )
    {
    }
    else
    {
        h->cards[h->size] = (guint8) card;
        h->size           = h->size + 1;
    }
}

void reset_hand (Hand *h)
{
    gint index;
    
    for (index = 0; index < MAX_HAND_SIZE; index++)
    {
        h->cards[index] = 0;
    }
    
    h->size = 0;
}

void remove_last_card (Hand *h)
{
    h->cards[h->size] = 0;
    h->size           = h->size - 1;
}
