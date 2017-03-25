#ifndef _R2BOT_JOB_HANDLER
#define _R2BOT_JOB_HANDLER

#include "Global.h"

#include "Controller.h"
#include "Job.h"
#include "Sensor.h"

#include <memory>

class JobHandler;

typedef ptr<JobHandler>(*JobHandlerParser)(string);

class JobHandler
{
protected:
	/** Types of job handlers and pointers to parsing function */
	static smap<JobHandlerParser> JobHandlers;
public:
	/** Register this job handler for running jobs */
	static void RegisterJobHandler(string handlerName, JobHandlerParser parser);

	JobHandler();
	virtual ~JobHandler();

	/** Runs the job handler's actions */
	virtual void execute(smap<ptr<Job>>& jobs, smap<ptr<SensorData>>& data, smap<string>& outputs);
};

#endif