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
 
#ifndef _CARDS_H_
#define _CARDS_H_

#include <glib.h>

#define TWO		                ((gint8) 2 )
#define THREE	                ((gint8) 3 )
#define FOUR	                ((gint8) 4 )
#define FIVE	                ((gint8) 5 )
#define SIX		                ((gint8) 6 )
#define SEVEN	                ((gint8) 7 )
#define EIGHT	                ((gint8) 8 )
#define NINE	                ((gint8) 9 )
#define TEN		                ((gint8) 10)
#define JACK	                ((gint8) 10)
#define QUEEN	                ((gint8) 10)
#define KING	                ((gint8) 10)
#define ACE		                ((gint8) 11)

#define NUM_VALUES				((gint8) 10)
#define NUM_CARDS 				((gint8) 13)
#define NUM_SUITS 				((gint8)  4)
#define NUM_CARDS_PER_DECK		((gint) (((gint)NUM_CARDS) * ((gint)NUM_SUITS)))
#define IS_CARD_ACE(c)          ( ((gint8)(c)) == ACE ? (TRUE) : (FALSE) )

#endif
