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

#ifndef _SM_H_
#define _SM_H_

typedef void (*funcptr)();
typedef funcptr (*StateFunc)();

/* States */

StateFunc init     (void);
StateFunc shuffle  (void);
StateFunc betting  (void);
StateFunc dealing  (void);
StateFunc players  (void);
StateFunc dealer   (void);
StateFunc payout   (void);
StateFunc finished (void);

#define FUNCTION_TO_STATE(f) ((StateFunc)(f))

#endif
