#ifndef _R2BOT_SPEECH_HANDLER
#define _R2BOT_SPEECH_HANDLER

#include "Global.h"
#include "JobHandler.h"

#include <deque>

class SpeechHandler : public JobHandler {
private:
	/** Register this job handler */
	static bool registered;

protected:
public:
	SpeechHandler();
	virtual ~SpeechHandler();

	virtual void execute(std::deque<Job>& jobs, SensorData& data, ControllerData& outputs);
};

#endif