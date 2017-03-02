#ifndef _JobHendler
#define _JobHendler

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
	virtual smap<ptr<Job>> execute(smap<void*> data);
};

#endif
