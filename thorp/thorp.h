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

#ifndef _THORP_H_
#define _THORP_H_

#include "../blackjack/blackjack.h"
#include "../blackjack/cards.h"
#include "../hand/hand.h"

#define MIN_VALUE_HARD		4
#define MIN_VALUE_SOFT		13
#define MIN_VALUE_PAIR		2
#define MIN_VALUE_UPCARD	2
#define PAIR_LENGTH         2

#define NUM_HARD_LUT		19
#define NUM_SOFT_LUT		10
#define NUM_PAIR_LUT		10
#define ACE_PAIR_ROW        7

typedef enum
{
	H = HIT,
	S = STAND,
	P = SPLIT,
#if (DOUBLE_ALLOWED != FALSE)
	D = DOUBLE,
	B = DOUBLE,
#else
	D = HIT,
	B = STAND,
#endif
#if (SURRENDER_ALLOWED != FALSE)
	R = SURRENDER,
	G = SURRENDER,
	T = SURRENDER,
#else
	R = HIT,
	G = STAND,
	T = SPLIT,
#endif
#if (DOUBLE_AFTER_SPLIT_ALLOWED != FALSE)
	L = SPLIT
#else
	L = HIT
#endif
} thorp_lut;

Action get_thorp_action (Hand *hand, gint upcard);
guint8 get_hand_value   (Hand *hand);

#endif
