/*
 * fir_filter.c
 *
 *  Created on: 25.11.2012
 *      Author & Copyright: bubi-007
 *          http://interactive-matter.eu/blog/2009/12/18/filtering-sensor-data-with-a-kalman-filter/
 *          Stripped down being a finite impulse response filter
 */
#include "stdint.h"
#include "fir_filter.h"

// accFilterStep configuration
#define FIR_ACC_Q       0.0625    // process noise covariance
#define FIR_ACC_R       1.0       // measurement noise covariance
#define FIR_ACC_P       0.22      // estimation error covariance

// gyroFilterStep configuration
#define FIR_GYRO_Q      1.0       // process noise covariance
#define FIR_GYRO_R      0.0625    // measurement noise covariance
#define FIR_GYRO_P      0.22      // estimation error covariance

typedef struct {
    float q;    // process noise covariance
    float r;    // measurement noise covariance
    float p;    // estimation error covariance
    float x;    // value
} firstate_t;


void firFilterInit(firstate_t* state, float q, float r, float p, float value)
{
    state->q = q;
    state->r = r;
    state->p = p;
    state->x = value;
}

int16_t firFilterUpdate(firstate_t* state, int16_t value)
{
    float measurement = value;
    float k; // kalman gain

    // prediction update
    state->p = state->p + state->q;

    // measurement update
    k = state->p / (state->p + state->r);
    state->x = state->x + k * (measurement - state->x);
    state->p = (1 - k) * state->p;

    return (int16_t)(state->x + 0.5f);
}

void accFirFilter(int16_t data[3])
{
    static int _init = 0;
    static firstate_t x;
    static firstate_t y;
    static firstate_t z;

    if (!_init) {
        _init = 1;
        firFilterInit(&x, FIR_ACC_Q, FIR_ACC_R, FIR_ACC_P, data[0]);
        firFilterInit(&y, FIR_ACC_Q, FIR_ACC_R, FIR_ACC_P, data[1]);
        firFilterInit(&z, FIR_ACC_Q, FIR_ACC_R, FIR_ACC_P, data[2]);
    } else {
        data[0] = firFilterUpdate(&x, data[0]);
        data[1] = firFilterUpdate(&y, data[1]);
        data[2] = firFilterUpdate(&z, data[2]);
    }
}

void gyroFirFilter(int16_t data[3])
{
    static firstate_t x;
    static firstate_t y;
    static firstate_t z;
    static int _init = 0;

    if (!_init) {
        _init = 1;
        firFilterInit(&x, FIR_GYRO_Q, FIR_GYRO_R, FIR_GYRO_P, data[0]);
        firFilterInit(&y, FIR_GYRO_Q, FIR_GYRO_R, FIR_GYRO_P, data[1]);
        firFilterInit(&z, FIR_GYRO_Q, FIR_GYRO_R, FIR_GYRO_P, data[2]);
    } else {
        data[0] = firFilterUpdate(&x, data[0]);
        data[1] = firFilterUpdate(&y, data[1]);
        data[2] = firFilterUpdate(&z, data[2]);
    }
}
