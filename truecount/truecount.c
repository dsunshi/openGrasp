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

#include "truecount.h" 

const gint8 count_lut[NUM_COUNT_TYPE][NUM_VALUES] =
{
	{		/* KO */
		1,  /* 02 */
		1,  /* 03 */
		1,  /* 04 */
		1,  /* 05 */
		1,  /* 06 */
		1,  /* 07 */
		0,  /* 08 */
		0,  /* 09 */
		-1, /* 10 */
		-1  /* 11 */
	},
	{		/* Hi/Low */
		1,  /* 02 */
		1,  /* 03 */
		1,  /* 04 */
		1,  /* 05 */
		1,  /* 06 */
		0,  /* 07 */
		0,  /* 08 */
		0,  /* 09 */
		-1, /* 10 */
		-1  /* 11 */
	},
	{		/* Hi Opt 1 */
		0,  /* 02 */
		1,  /* 03 */
		1,  /* 04 */
		1,  /* 05 */
		1,  /* 06 */
		0,  /* 07 */
		0,  /* 08 */
		0,  /* 09 */
		-1, /* 10 */
		0   /* 11 */
	},
	{		/* Hi Opt 2 */
		1,  /* 02 */
		1,  /* 03 */
		2,  /* 04 */
		2,  /* 05 */
		1,  /* 06 */
		1,  /* 07 */
		0,  /* 08 */
		0,  /* 09 */
		-2, /* 10 */
		0   /* 11 */
	},
	{		/* Zen */
		1,  /* 02 */
		1,  /* 03 */
		2,  /* 04 */
		2,  /* 05 */
		2,  /* 06 */
		1,  /* 07 */
		0,  /* 08 */
		0,  /* 09 */
		-2, /* 10 */
		-1  /* 11 */
	},
	{		/* Omega II */
		1,  /* 02 */
		1,  /* 03 */
		2,  /* 04 */
		2,  /* 05 */
		2,  /* 06 */
		1,  /* 07 */
		0,  /* 08 */
		-1, /* 09 */
		-2, /* 10 */
		0   /* 11 */
	}
};

const gint8 cards_per_type[NUM_VALUES] = 
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

void reset_true_count (TrueCount *count)
{
    gint index;
    
    for (index = 0; index < NUM_COUNT_TYPE; index++)
    {
        count->counts[index] = 0;
    }
}

TrueCount *create_true_count (void)
{
    TrueCount *count = NULL;
    
    count = g_new(TrueCount, 1);
    
    if (count == NULL)
    {
    }
    else
    {
        count->counts = g_new(gint, NUM_COUNT_TYPE);
        
        if (count->counts == NULL)
        {
        }
        else
        {
            reset_true_count(count);
        }
    }
    
    return count;
}

void destroy_true_count(TrueCount *count)
{
    g_free( count->counts );
    g_free( count );
}

void  update_true_count (TrueCount *count, gint8 card)
{
    gint index;
    
    for (index = 0; index < NUM_COUNT_TYPE; index++)
	{
		count->counts[index] += count_lut[index][card - 2];	
	}
}

gint get_true_count (TrueCount *count, gint8 type)
{
    gint result;
    
    if (type >= NUM_COUNT_TYPE)
    {
        result = -1;
    }
    else
    {
        result = CLAMP( count->counts[type], TRUE_COUNT_MIN, TRUE_COUNT_MAX );
    }
    
    return result;
}
