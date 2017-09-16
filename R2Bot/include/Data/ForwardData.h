#ifndef _R2BOT_FORWARD_DATA
#define _R2BOT_FORWARD_DATA

#include "Global.h"
#include "R2Protocol.hpp"

typedef struct {
	smap<std::vector<R2Protocol::Packet>> data;
} ForwardData;

#endif