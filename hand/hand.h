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

#ifndef _HAND_H_
#define _HAND_H_

#include <glib.h>

#define MAX_HAND_SIZE   15

typedef struct hand_t
{
    guint8 *cards;
    guint8  size;
} Hand;

Hand *create_hand  (void);
void  destroy_hand (Hand *h);

void   receive_card   (Hand *h, gint card);
guint8 get_card_value (Hand *h, gint card);

void reset_hand    (Hand *h);
void remove_last_card (Hand *h);

#endif