#ifndef _R2BOT_ROAMER_HANDLER
#define _R2BOT_ROAMER_HANDLER

#include "Global.h"
#include "JobHandler.h"
#include "Data/UltrasoundData.h"
#include "Data/MotorData.h"

#include <chrono>
#include <deque>

class RoamerHandler: public JobHandler{
private:
    static bool registered;
    bool lb, rb;
    std::chrono::time_point<std::chrono::steady_clock> lastActivity;

public:
    RoamerHandler();
    virtual ~RoamerHandler();
    virtual void execute(std::deque<Job>& jobs, SensorData& data, ControllerData& outputs);
};

#endif
