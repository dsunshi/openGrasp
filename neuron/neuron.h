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

#ifndef _GENE_H_
#define _GENE_H_

#include <glib.h>

#define NEURON_OUTPUT_HIGH  1
#define NEURON_OUTPUT_LOW   0

typedef struct neuron_values_t
{
    gint *w;
    gint *x;
    gint  threshold;
} NValue;

typedef struct neuron_t
{
    NValue *value;
    gint    size;
    gint    output;
} Neuron;

void calc_neuron_output (Neuron *n);

void destory_neuron   (Neuron *n);
Neuron *create_neuron (const gint size);

#endif