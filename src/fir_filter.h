/*
 * fir_filter.h
 *
 *  Created on: 25.11.2012
 *      Author: bubi-007
 */

#ifndef FIR_FILTER_H_
#define FIR_FILTER_H_

void accFirFilter(int16_t acc[3]);
void gyroFirFilter(int16_t gyros[3]);

#endif /* FIR_FILTER_H_ */