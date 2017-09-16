#ifndef _R2BOT_SOUND_HANDLER
#define _R2BOT_SOUND_HANDLER

#include "Global.h"
#include "JobHandler.h"

#include <deque>

class SoundHandler : public JobHandler {
private:
	/** Register this job handler */
	static bool registered;

protected:
public:
	SoundHandler();
	virtual ~SoundHandler();

	virtual void execute(std::deque<Job>& jobs, SensorData& data, ControllerData& outputs);
};

#endif