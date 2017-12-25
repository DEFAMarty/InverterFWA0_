#ifndef SOLAR_IQ_H
#define SOLAR_IQ_H

#ifndef DSP28_DATA_TYPES
#define DSP28_DATA_TYPES
typedef int             	int16;
typedef long            	int32;
typedef long long			int64;
typedef unsigned int		Uint16;
typedef unsigned long		Uint32;
typedef unsigned long long	Uint64;
typedef float           	float32;
typedef long double     	float64;
#endif

#include "IQmathLib.h"

#include "CNTL_2P2Z_IQ.h"
#include "CNTL_3P3Z_IQ.h"
#include "CNTL_PI_IQ.h"
#include "RAMPGEN_IQ.h"
#include "SINEANALYZER_DIFF_IQ.h"
#include "SINEANALYZER_DIFF_wPWR_IQ.h"


#endif
