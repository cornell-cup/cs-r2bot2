#ifndef _R2BOT_LIDAR_DATA
#define _R2BOT_LIDAR_DATA

#include "Global.h"

typedef struct {
	std::vector<double> angles, distances;
} LidarData;

#endif