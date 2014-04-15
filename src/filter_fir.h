/*
 * fir_filter.h
 *
 *  Created on: 25.11.2012
 *      Author: bubi-007
 */

#ifndef FIR_FILTER_H_
#define FIR_FILTER_H_

typedef struct {
    float q;    // process noise covariance
    float r;    // measurement noise covariance
    float e;    // estimation error covariance
    float x;    // value
} firstate_t;

typedef struct {
    firstate_t x,y,z;   // axes
    int start;          // filter state
} firvect_t;


void firFilter(int16_t acc[3], firvect_t* firvect);


#endif /* FIR_FILTER_H_ */
