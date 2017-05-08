#ifndef _R2BOT_POWER_DATA
#define _R2BOT_POWER_DATA

#include "Global.h"

typedef struct {
	float voltage;
	float current;
	float batteryCapacity;
	float batteryLevel;
} PowerData;

#endif