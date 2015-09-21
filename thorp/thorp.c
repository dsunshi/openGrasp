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

#include "thorp.h"

const thorp_lut Hard[NUM_HARD_LUT][NUM_VALUES] =
{
  /* 2 3 4 5 6 7 8 9 T A   */
	{H,H,H,H,H,H,H,H,H,H}, /* 04 */
	{H,H,H,H,H,H,H,H,H,H}, /* 05 */
	{H,H,H,H,H,H,H,H,H,H}, /* 06 */
	{H,H,H,H,H,H,H,H,H,H}, /* 07 */
	{H,H,H,H,H,H,H,H,H,H}, /* 08 */
	{H,D,D,D,D,H,H,H,H,H}, /* 09 */
	{D,D,D,D,D,D,D,D,H,H}, /* 10 */
	{D,D,D,D,D,D,D,D,D,D}, /* 11 */
	{H,H,S,S,S,H,H,H,H,H}, /* 12 */
	{S,S,S,S,S,H,H,H,H,H}, /* 13 */
	{S,S,S,S,S,H,H,H,H,H}, /* 14 */
	{S,S,S,S,S,H,H,H,R,R}, /* 15 */
	{S,S,S,S,S,H,H,R,R,R}, /* 16 */
	{S,S,S,S,S,S,S,S,S,G}, /* 17 */
	{S,S,S,S,S,S,S,S,S,S}, /* 18 */
	{S,S,S,S,S,S,S,S,S,S}, /* 19 */
	{S,S,S,S,S,S,S,S,S,S}, /* 20 */
	{S,S,S,S,S,S,S,S,S,S}, /* 21 */
	{S,S,S,S,S,S,S,S,S,S}  /* 22 */
};

const thorp_lut Soft[NUM_SOFT_LUT][NUM_VALUES] = 
{
  /* 2 3 4 5 6 7 8 9 T A   */
	{H,H,H,D,D,H,H,H,H,H}, /* 13 */
	{H,H,H,D,D,H,H,H,H,H}, /* 14 */
	{H,H,D,D,D,H,H,H,H,H}, /* 15 */
	{H,H,D,D,D,H,H,H,H,H}, /* 16 */
	{H,D,D,D,D,H,H,H,H,H}, /* 17 */
	{B,B,B,B,B,S,S,H,H,H}, /* 18 */
	{S,S,S,S,B,S,S,S,S,S}, /* 19 */
	{S,S,S,S,S,S,S,S,S,S}, /* 20 */
	{S,S,S,S,S,S,S,S,S,S}, /* 21 */
	{S,S,S,S,S,S,S,S,S,S}  /* 22 */
};

const thorp_lut Split[NUM_PAIR_LUT][NUM_VALUES] =
{
  /* 2 3 4 5 6 7 8 9 T A   */
	{L,L,P,P,P,P,H,H,H,H}, /* 2,2 */
	{L,L,P,P,P,P,H,H,H,H}, /* 3,3 */
	{H,H,H,L,L,H,H,H,H,H}, /* 4,4 */
	{D,D,D,D,D,D,D,D,H,H}, /* 5,5 (10) */
	{L,P,P,P,P,H,H,H,H,H}, /* 6,6 */
	{P,P,P,P,P,P,H,H,H,H}, /* 7,7 */
	{P,P,P,P,P,P,P,P,P,T}, /* 8,8 */
	{P,P,P,P,P,S,P,P,S,S}, /* 9,9 */
	{S,S,S,S,S,S,S,S,S,S}, /* 10,10 (20) */
	{P,P,P,P,P,P,P,P,P,P}  /* A,A */
};

static void tally (Hand *hand, guint8 *num_aces, guint8 *sum);
  
static void tally (Hand *hand, guint8 *num_aces, guint8 *sum)
{
	guint8  count;
	guint8  total;
	gint    index;
	
	count = 0;
	total = 0;
	
	for (index = 0; index < hand->size; index++)
	{
		if (IS_CARD_ACE( get_card_value(hand, index) ) != FALSE)
		{
			count++;
		}
		else
		{
			/* Do nothing, (not an ACE). */
		}
	
		total += get_card_value(hand, index);
	}
	
	*num_aces = count;
	*sum      = total;
}

guint8 get_hand_value (Hand *hand)
{
	guint8  num_aces;
	guint8  sum;
	
	tally(hand, &num_aces, &sum);
	
	for (; (sum > BLACKJACK) && (num_aces > 0); num_aces--)
	{
		sum -= SOFT_TO_HARD_ACE;
	}
	
	if (sum > BLACKJACK)
	{
		/* Saturate 'Bust' values */
		sum = BUST;
	}
	else
	{
		/* Do nothing */
	}
	
	return sum;
}

Action get_thorp_action (Hand *hand, gint upcard)
{
	Action  result;
	guint8  num_aces;
	guint8  sum;
	guint8  row;
	guint8  col;

    g_assert_cmpint(upcard, <=, ACE);
    g_assert_cmpint(upcard, >=, MIN_VALUE_UPCARD);
	
	col = upcard - MIN_VALUE_UPCARD;
	
	tally(hand, &num_aces, &sum);
	
	for (; (sum > BLACKJACK) && (num_aces > 0); num_aces--)
	{
		sum -= SOFT_TO_HARD_ACE;
	}
	
	if (sum > BLACKJACK)
	{
		/* Saturate 'Bust' values */
		sum = BUST;
	}
	else
	{
		/* Do nothing */
	}
	
	if ( (num_aces > 0) && (hand->size != PAIR_LENGTH) )
	{
		/* Soft, and not a pair of aces */
		/*ASSERT_INCLUSIVE_RANGE(sum, MIN_VALUE_SOFT, BUST);*/
		
		row = sum - MIN_VALUE_SOFT;
		
		/*ASSERT_ARRAY_RANGE(row, NUM_SOFT_LUT);*/
		
		result = Soft[row][col];
	}
	else
	{
		/* Hard */
		/*ASSERT_INCLUSIVE_RANGE(sum, MIN_VALUE_HARD, BUST);*/
		
		row = sum - MIN_VALUE_HARD;
		
		/*ASSERT_ARRAY_RANGE(row, NUM_HARD_LUT);*/
		
		result = Hard[row][col];
  
		if (hand->size == PAIR_LENGTH)
		{
			if ( get_card_value(hand, 0) == get_card_value(hand, 1) )
			{
				/* Pair */  
				/*assert(  (sum / 2) >= MIN_VALUE_PAIR );*/
				
				if (IS_CARD_ACE( get_card_value(hand, 0) ) != FALSE)
				{
					/* Pair of Aces */
					row = ACE_PAIR_ROW;
				}
				else
				{
					/*assert( (sum / 2) <= NUM_PAIR_LUT );*/
					/* Non-Ace pair */
					row = (sum >> 1) - MIN_VALUE_PAIR;
				}
			
				/*ASSERT_ARRAY_RANGE(row, NUM_PAIR_LUT);*/
			
				result = Split[row][col];
			}
			else
			{
                /* Hard, (still) */
			}
		}
		else
		{
			/* Hard, (still) */
		}
	}
    
    return result;
}
