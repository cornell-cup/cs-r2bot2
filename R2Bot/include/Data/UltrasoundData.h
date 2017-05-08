#ifndef _R2BOT_ULTRASOUND_DATA
#define _R2BOT_ULTRASOUND_DATA

#include "Global.h"
#include <vector>

typedef struct {
	std::vector<float> distance;
} UltrasoundData;

#endif