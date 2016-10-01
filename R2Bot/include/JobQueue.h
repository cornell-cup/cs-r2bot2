#ifndef _JobQueue
#define _JobQueue

#include <string>
#include <queue>
#include <fstream>

class JobQueue
{
protected:
	std::string hostName;
	int portNumber;
	std::queue<std::string> jobs;

	/** Receive job commands to place on the queue */
	void receiveJobs();

public:
	JobQueue();
	JobQueue(std::string jobsFile, std::string host, int port);
	~JobQueue();

	/** Adds a network interface on which to listen for jobs */
	void setNetworkInterface(std::string host, int port);

	/** Adds jobs to queue from given file */
	void addJobsFile(std::string filename);

	/** Places oldest job in str and returns true if there was a job to put */
	bool getJob(std::string *str);
};

#endif
