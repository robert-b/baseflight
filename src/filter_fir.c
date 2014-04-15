/*
 * filter_fir.c
 *
 *  Created on: 25.11.2012
 *      Author & Copyright: bubi-007
 *          http://interactive-matter.eu/blog/2009/12/18/filtering-sensor-data-with-a-kalman-filter/
 *          Stripped down being a finite impulse response filter
 */
#include <math.h>
#include "stdint.h"
#include "filter_fir.h"

#define FILTER_INIT_VALUE(S, DATA) ((firstate_t*)(&S->x))->value = DATA[0]; ((firstate_t*)(&S->y))->value = DATA[1];((firstate_t*)(&S->z))->value = DATA[2];S->start = 1;

int16_t firFilterUpdate(firstate_t* state, int16_t measurement)
{
    float k; // kalman gain

    // prediction update
    state->p = state->p + state->q;

    // measurement update
    k = state->p / (state->p + state->r);
    state->value = state->value + k * (measurement - state->value);
    state->p = (1 - k) * state->p;

    return lrintf(state->value);
}

void firFilter(int16_t data[3], firvect_t* stat)
{
    if (stat->start) {
        data[0] = firFilterUpdate(&stat->x, data[0]);
        data[1] = firFilterUpdate(&stat->y, data[1]);
        data[2] = firFilterUpdate(&stat->z, data[2]);
        return;
    }

    FILTER_INIT_VALUE(stat, data)
}

