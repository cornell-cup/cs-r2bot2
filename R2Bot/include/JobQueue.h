#ifndef _JobQueue
#define _JobQueue

#include <string>
#include <queue>

class JobQueue
{
protected:
	std::string hostName;
	int portNumber;
	std::queue<std::string> jobs;

	/** Receive job commands to place on the queue */
	void receiveJobs();

public:
	JobQueue(std::string host, int port);
	~JobQueue();

	/** Places oldest job in str and returns true if there was a job to put */
	bool getJob(std::string *str);
};

#endif
