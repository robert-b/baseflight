/*
 * fir_filter.h
 *
 *  Created on: 25.11.2012
 *      Author: bubi-007
 */

#ifndef FIR_FILTER_H_
#define FIR_FILTER_H_

#define FIR_FILTER_INIT(S, s1, s2, s3) S.q=s1; S.r=s2; S.p=s3;


// accFilterStep configuration
#define FIR_ACC_Q       1.0f        // process noise covariance
#define FIR_ACC_R       0.06f       // measurement noise covariance
#define FIR_ACC_P       0.22f       // estimation error covariance

// gyroFilterStep configuration
#define FIR_GYRO_Q      1.0f        // process noise covariance
#define FIR_GYRO_R      0.06f       // measurement noise covariance
#define FIR_GYRO_P      0.22f       // estimation error covariance



typedef struct {
    float q;    // process noise covariance
    float r;    // measurement noise covariance
    float p;    // estimation error covariance
    float value;    // value
} firstate_t;

typedef struct {
    firstate_t x,y,z;   // axes
    int start;          // filter state
} firvect_t;


void firFilter(int16_t acc[3], firvect_t* firvect);


#endif /* FIR_FILTER_H_ */
