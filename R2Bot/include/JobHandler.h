#ifndef _R2BOT_JOB_HANDLER
#define _R2BOT_JOB_HANDLER

#include "Global.h"

#include "Controller.h"
#include "Job.h"
#include "Sensor.h"

#include <memory>

class JobHandler
{
protected:
public:
	JobHandler();
	virtual ~JobHandler();

	/** Runs the job handler's actions */
	virtual void execute(smap<ptr<Job>>& jobs, smap<ptr<SensorData>>& data, smap<string>& outputs);
};

#endif