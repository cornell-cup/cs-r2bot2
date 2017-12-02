#ifndef _ROAMER_HANDLER
#define _ROAMER_HANDLER

#include "JobHandler.h"
#include "Data/UltrasoundData.h"
#include "Data/MotorData.h"

class RoamerHandler: public JobHandler{
public:
    virtual void execute(std::deque<Job>& jobs, SensorData& data, ControllerData& outputs);
};

#endif