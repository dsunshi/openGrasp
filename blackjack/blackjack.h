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

#ifndef _BLACKJACK_H_
#define _BLACKJACK_H_

#include <glib.h>

typedef guint8 Action;

#define HIT					((Action) 0)
#define STAND				((Action) 1)
#define DOUBLE				((Action) 2)
#define SPLIT				((Action) 3)
#define SURRENDER			((Action) 4)

#define BLACKJACK			21
#define BUST				22

#define HARD				0
#define SOFT				1
#define PAIR				2

#define SOFT_TO_HARD_ACE	10


/* Game play related configurations */
#define DOUBLE_ALLOWED              FALSE
#define SURRENDER_ALLOWED           FALSE
#define DOUBLE_AFTER_SPLIT_ALLOWED  FALSE

#endif
