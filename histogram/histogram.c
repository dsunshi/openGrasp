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

#include "histogram.h"

#define NUM_UNIQ_CARD_VALUES    ((gint8) 10)

static const gint8 cards_per_type[NUM_UNIQ_CARD_VALUES] = 
{
	4,  /* 02 */
	4,  /* 03 */
	4,  /* 04 */
	4,  /* 05 */
	4,  /* 06 */
	4,  /* 07 */
	4,  /* 08 */
	4,  /* 09 */
	16, /* 10 */
	4   /* 11 */
};

Histogram *create_histogram( gint num_decks )
{
    Histogram  *hist;
    
    hist = g_new(Histogram, 1);
    
    if (hist == NULL)
    {
        g_error("Failed to allocated memory for histogram\n");
    }
    else
    {
        hist->histogram = g_new0(gint, NUM_UNIQ_CARD_VALUES);
        
        if (hist->histogram  == NULL)
        {
        }
        else
        {
			reset_histogram( hist );
            hist->num_decks = num_decks;
        }
    }
    
    return hist;
}

void update_histogram( Histogram *hist, gint8 card )
{
    hist->histogram[card - TWO]++;
}

gint get_histogram_count( Histogram *hist, gint8 card )
{
    return hist->histogram[card - TWO];
}

gint get_bust_odds( Histogram *hist, gint8 value )
{
	gint percent_failure = 0;
	gint index;
    gint count;
    
    gint bust_value = BUST - value;
    gint num_ok     = 0;
    gint num_bust   = 0;

    if ( value >= 21 )
    {
        /* If we have busted already or have a blackjack, then the bust odds are 100% */
        percent_failure = PERCENT;
    }
	else if (bust_value > 10)
	{
		/* If need more than 10 to bust than is cannot happen so then the bust odds are 0% */
        percent_failure = 0;
	}
    else
    {
        for (index = TWO; index < bust_value; index++)
        {
            count   = get_histogram_count(hist, index);
            num_ok += (cards_per_type[index - TWO] * hist->num_decks) - count;
        }
        
        /* 
         * An ace has a value of one, so the only time an ace would cause a bust
         * is if we hit on 21, and this is not a valid thing to do. Therefore, ace's
         * are always OK.
         */
        count   = get_histogram_count(hist, ACE);
        num_ok += (cards_per_type[ACE - TWO] * hist->num_decks) - count;
        
        for (; index <= TEN; index++)
        {
            count     = get_histogram_count(hist, index);
            num_bust += (cards_per_type[index - TWO] * hist->num_decks) - count;
        }
        
        percent_failure = (num_bust * PERCENT) / (num_bust + num_ok);
    }

	return percent_failure;
}

void reset_histogram ( Histogram *hist )
{
	gint index;
    
    for (index = 0; index < NUM_UNIQ_CARD_VALUES; index++)
    {
        hist->histogram[index] = 0;
    }
}

void destory_histogram ( Histogram *hist )
{
	g_free( hist->histogram );
    g_free( hist );
}
