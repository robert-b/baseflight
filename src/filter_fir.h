#ifndef FIR_FILTER_H_
#define FIR_FILTER_H_

#define FIR_FILTER_INIT(S, s1, s2, s3) S.q=s1; S.r=s2; S.p=s3;

// accFilterStep default configuration
#define FIR_ACC_Q       1.0f
#define FIR_ACC_R       0.06f
#define FIR_ACC_P       0.22f

// gyroFilterStep default configuration
#define FIR_GYRO_Q      1.0f
#define FIR_GYRO_R      0.06f
#define FIR_GYRO_P      0.22f

typedef struct firState_t {
    float q;        // process noise covariance
    float r;        // measurement noise covariance
    float p;        // estimation error covariance
    float value;    // value
} firState_t;

typedef struct firVector_t {
    firState_t x, y, z; // axes
    int start;          // true if the filter can start
} firVector_t;

void firFilter(int16_t acc[3], firVector_t* firvect);

#endif
