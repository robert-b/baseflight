/*
 * filter_fir.c
 *
 *  Stripped down kalman filter being a finite impulse response filter
 *  http://interactive-matter.eu/blog/2009/12/18/filtering-sensor-data-with-a-kalman-filter/
 *
 */
#include <math.h>
#include "stdint.h"
#include "filter_fir.h"

#define FIR_FILTER_INIT_VALUE(S, DATA) (&S->x)->value = DATA[0]; (&S->y)->value = DATA[1];(&S->z)->value = DATA[2];S->start = 1;

int16_t firUpdate(firState_t* state, int16_t measurement)
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

void firFilter(int16_t data[3], firVector_t* states)
{
    if (states->start) {
        data[0] = firUpdate(&states->x, data[0]);
        data[1] = firUpdate(&states->y, data[1]);
        data[2] = firUpdate(&states->z, data[2]);
        return;
    }
    FIR_FILTER_INIT_VALUE(states, data)
}

