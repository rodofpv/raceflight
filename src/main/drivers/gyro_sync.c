/*
 * gyro_sync.c
 *
 *  Created on: 3 aug. 2015
 *      Author: borisb
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "platform.h"
#include "build_config.h"

#include "common/axis.h"
#include "common/maths.h"

#include "drivers/sensor.h"
#include "drivers/accgyro.h"
#include "drivers/gyro_sync.h"

#include "sensors/sensors.h"
#include "sensors/acceleration.h"

#include "config/runtime_config.h"
#include "config/config.h"

extern gyro_t gyro;

uint32_t targetLooptime;
static uint8_t mpuDividerDrops;
static uint8_t gyroFilterRate;

bool getMpuDataStatus(gyro_t *gyro)
{
    bool mpuDataStatus;

    gyro->intStatus(&mpuDataStatus);
    return mpuDataStatus;
}

bool gyroSyncCheckUpdate(void) {
    return getMpuDataStatus(&gyro);
}

void gyroUpdateSampleRate(uint8_t lpf) {

    int gyroFrequency;
    int gyroSampleRate;

#if defined(REVONANO) || defined(SPARKY2) || defined(ALIENFLIGHTF4) || defined(BLUEJAYF4) || defined(VRCORE)

    if (lpf < 2) {
        gyroFrequency  = 31; // gyro sampling rate 32khz
        gyroSampleRate = 31; // 32khz sampling, full sampling
        targetLooptime = 1000;  // Wanted looptime
    } else if (lpf < 3) {
    	gyroFrequency  = 31; // gyro sampling rate 32khz
        gyroSampleRate = 31; // 32khz sampling, full sampling
        targetLooptime = 500;  // Wanted looptime
    } else if (lpf < 5) {
    	gyroFrequency  = 31; // gyro sampling rate 32khz
        gyroSampleRate = 31; // 32khz sampling, full sampling
        targetLooptime = 250;  // Wanted looptime
    } else if (lpf < 9) {
    	gyroFrequency  = 31; // gyro sampling rate 32khz
        gyroSampleRate = 31; // 32khz sampling, full sampling
        targetLooptime = 125;  // Wanted looptime
    } else if (lpf < 16) {
    	gyroFrequency  = 31; // gyro sampling rate 32khz
        gyroSampleRate = 31; // 32khz sampling, full sampling
        targetLooptime = 62;  // Wanted looptime
    } else {
    	gyroFrequency  = 31; // gyro sampling rate 32khz
        gyroSampleRate = 31; // 32khz sampling, full sampling
        targetLooptime = 62;  // Wanted looptime
        //targetLooptime = 31;  // Wanted looptime
    }

#else

    if (lpf < 2) {
		gyroFrequency  = 125; // gyro sampling rate 8khz
		gyroSampleRate = 125; // 8khz sampling, full sampling
		targetLooptime = 1000;  // Wanted looptime
	} else if (lpf < 3) {
		gyroFrequency  = 125; // gyro sampling rate 8khz
		gyroSampleRate = 125; // 8khz sampling, full sampling
		targetLooptime = 500;  // Wanted looptime
	} else if (lpf < 5) {
		gyroFrequency  = 125; // gyro sampling rate 8khz
		gyroSampleRate = 125; // 8khz sampling, full sampling
		targetLooptime = 250;  // Wanted looptime
	} else if (lpf < 9) {
		gyroFrequency  = 125; // gyro sampling rate 8khz
		gyroSampleRate = 125; // 8khz sampling, full sampling
		targetLooptime = 125;  // Wanted looptime
	}

#endif

    // calculate gyro divider and targetLooptime (expected cycleTime)
    mpuDividerDrops = ( gyroSampleRate / gyroFrequency ) - 1;
    gyroFilterRate  = ( targetLooptime / gyroSampleRate );

}

uint8_t gyroMPU6xxxGetDividerDrops(void) {
    return mpuDividerDrops;
}
