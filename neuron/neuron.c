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

#include "neuron.h"

static gint sum_neuron ( Neuron *n );

static gint sum_neuron ( Neuron *n )
{
    gint sum = 0;
    gint index;
    
    for (index = 0; index < n->size; index++)
    {
        sum += n->value->w[index] * n->value->x[index];
    }
    
    return sum;
}

void calc_neuron_output (Neuron *n)
{
    gint sum;
    
    sum = sum_neuron( n );
    
    if (sum > n->value->threshold)
    {
        n->output = NEURON_OUTPUT_HIGH;
    }
    else
    {
        n->output = NEURON_OUTPUT_LOW;
    }
}

void destory_neuron (Neuron *n)
{
    g_free( n->value->x );
    g_free( n->value->w );
    g_free( n->value );
    g_free( n );
}

Neuron *create_neuron  (const gint size)
{
    Neuron *neuron;
    NValue *value;
    
    neuron = g_new(Neuron, 1);
    value  = g_new(NValue, 1);
    
    if ( (neuron == NULL) || (value == NULL) )
    {
        g_error("Neuron-CRITICAL **: Could not allocate memory for Neuron\n");
    }
    else
    {
        value->x = g_new(gint, size);
        value->w = g_new(gint, size);
        
        if ( (value->x == NULL) || (value->w == NULL) )
        {
            g_error("Neuron-CRITICAL **: Could not allocate memory for Neuron\n");
        }
        else
        {
            value->threshold = 0;
            
            neuron->size     = size;
            neuron->output   = 0;
            neuron->value    = value;
        }
    }
    
    return neuron;
}