#ifndef _R2BOT_JOB
#define _R2BOT_JOB

#include <string>

class Job {
protected:
	/** The name of the handler for this job */
	std::string handler;

public:
	Job(std::string handler);
	virtual ~Job();

	std::string getHandler();
};

#endif