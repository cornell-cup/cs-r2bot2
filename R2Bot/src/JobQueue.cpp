#include "JobQueue.h"

JobQueue::JobQueue() : hostName(""), portNumber(0)
{
}

JobQueue::JobQueue(std::string jobsFile, std::string host, int port) : hostName(host), portNumber(port)
{
	// Add all jobs from initial jobFile if provided
	if (jobsFile != "") {
		std::ifstream file(jobsFile);
		std::string line;
		while (std::getline(file, line))
			jobs.push(line);
	}

	// TODO: JobQueue
}

JobQueue::~JobQueue()
{
}

bool JobQueue::getJob(std::string * str)
{
	// TODO: getJob
	return false;
}

void JobQueue::receiveJobs()
{
	// TODO: receiveCommands
}
