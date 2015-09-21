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

#include <glib.h>
#include <stdio.h>

#include "shoe.h"

#define NUM_INTERNAL_SHUFFLE    7
#define ARRAY_STORAGE_TYPE      gint8

static gint garray_random_sortfunc ( gconstpointer a, gconstpointer b, gpointer user_data );

static gint garray_random_sortfunc ( gconstpointer a, gconstpointer b, gpointer user_data )
{
    GRand  *random = (GRand *) user_data;
    gint    r      = 0;
	
	if (g_rand_boolean(random) != FALSE)
	{
		r = 1;
	}
	else
	{
		r = -1;
	}
	
    return r;
}

void shuffle_shoe ( Shoe *shoe )
{
    gint index;
    
    if (shoe == NULL)
    {
        g_warning( "Shoe is NULL ... returning" );
    }
    else
    {
        for (index = 0; index < NUM_INTERNAL_SHUFFLE; index++)
        {
            g_array_sort_with_data( shoe->array, (GCompareDataFunc) garray_random_sortfunc, shoe->random );
        }
        
        shoe->current = 0;
        
        if ( shoe->shuffle_notify_callback != NULL )
        {
            shoe->shuffle_notify_callback();
        }
        else
        {
        
        }
    }
}

Shoe *create_shoe ( const unsigned int num_decks )
{
	unsigned int card_index = 0;
	unsigned int deck_index = 0;
	unsigned int suit_index = 0;
    
	gint8 value;
    
    Shoe *shoe  = NULL;
    shoe        = g_new( Shoe, 1 );
    
    if (shoe == NULL)
    {
        g_error("Failed to allocated memory for shoe\n");
    }
    else
    {
        shoe->num_cards = num_decks * NUM_CARDS_PER_DECK;
        shoe->array     = g_array_sized_new(TRUE, TRUE, sizeof(ARRAY_STORAGE_TYPE), shoe->num_cards);
        
        for (deck_index = 0; deck_index < num_decks; deck_index++)
        {
            for (suit_index = 0; suit_index < NUM_SUITS_PER_DECK; suit_index++)
            {
                for (card_index = TWO; card_index <= NINE; card_index++)
                {
                    value = (gint8) card_index;
                    g_array_append_val(shoe->array, value);
                }
                
                value = TEN;
                for (card_index = 0; card_index < NUM_TEN_VAL_PER_SUIT; card_index++)
                {
                    g_array_append_val(shoe->array, value);
                }
                
                value = ACE;
                g_array_append_val(shoe->array, value);
            }
        }
        
        shoe->current = 0;
        shoe->random  = g_rand_new();
        shoe->shuffle_notify_callback = NULL;
    }
	
	return shoe;
}

void destroy_shoe ( Shoe *shoe )
{
    g_array_free( shoe->array, TRUE );
    g_rand_free( shoe->random );
    g_free( shoe );
}

gint8 get_card ( Shoe *shoe )
{
    gint8 card = CARD_SHUFFLE_REQUIRED;
    
    if (shoe == NULL)
    {
        g_warning( "Shoe is NULL ... returning: CARD_SHUFFLE_REQUIRED" );
    }
    else
    {
        if (shoe->current == shoe->num_cards)
        {
            card = CARD_SHUFFLE_REQUIRED;

            if ( shoe->shuffle_notify_callback != NULL )
            {
                shoe->shuffle_notify_callback();
            }
            else
            {
            
            }
        }
        else
        {
            card = g_array_index( shoe->array, ARRAY_STORAGE_TYPE, shoe->current);
            shoe->current += 1;
        }
    }
    
    return card;
}

gint8 get_card_auto_reshuffle ( Shoe *shoe )
{
    gint8 card = CARD_SHUFFLE_REQUIRED;

    if (shoe == NULL)
    {
        g_warning( "Shoe is NULL ... returning: CARD_SHUFFLE_REQUIRED" );
    }
    else
    {
        card = get_card(shoe);
        
        if (card == CARD_SHUFFLE_REQUIRED)
        {
            shuffle_shoe( shoe );
            card = get_card_auto_reshuffle( shoe );
        }
        else
        {
            /* Do nothing, no shuffle is needed */
        }
    }
    
    return card;
}
