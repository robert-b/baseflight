/*
 * fir_filter.c
 *
 *  Created on: 25.11.2012
 *      Author & Copyright: bubi-007
 *          http://interactive-matter.eu/blog/2009/12/18/filtering-sensor-data-with-a-kalman-filter/
 *          Stripped down being a finite impulse response filter
 */
#include <math.h>
#include "stdint.h"
#include "filter_fir.h"

// accFilterStep configuration
#define FIR_ACC_Q       0.0625f    // process noise covariance
#define FIR_ACC_R       0.06f       // measurement noise covariance
#define FIR_ACC_P       0.22f      // estimation error covariance

// gyroFilterStep configuration
#define FIR_GYRO_Q      1.0f       // process noise covariance
#define FIR_GYRO_R      0.06f    // measurement noise covariance
#define FIR_GYRO_P      0.22f      // estimation error covariance


#define FILTER_INIT(S, s1, s2, s3, s4) ((firstate_t*)S)->q=s1; ((firstate_t*)S)->r=s2; ((firstate_t*)S)->e=s3; ((firstate_t*)S)->x=s4;


int16_t firFilterUpdate(firstate_t* state, int16_t measurement)
{
    float k; // kalman gain

    // prediction update
    state->e = state->e + state->q;

    // measurement update
    k = state->e / (state->e + state->r);
    state->x = state->x + k * (measurement - state->x);
    state->e = (1 - k) * state->e;

    return lrintf(state->x);
}

void firFilter(int16_t data[3], firvect_t* firvect)
{

    if (!firvect->state) {
        firvect->state = 1;
        FILTER_INIT(&firvect->x, FIR_ACC_Q, FIR_ACC_R, FIR_ACC_P, data[0])
        FILTER_INIT(&firvect->y, FIR_ACC_Q, FIR_ACC_R, FIR_ACC_P, data[1])
        FILTER_INIT(&firvect->z, FIR_ACC_Q, FIR_ACC_R, FIR_ACC_P, data[2])
    } else {
        data[0] = firFilterUpdate(&firvect->x, data[0]);
        data[1] = firFilterUpdate(&firvect->y, data[1]);
        data[2] = firFilterUpdate(&firvect->z, data[2]);
    }
}

