#include "JobQueue.h"

JobQueue::JobQueue() : hostName(""), portNumber(0)
{
}

JobQueue::JobQueue(std::string jobsFile, std::string host, int port) : hostName(host), portNumber(port)
{
	// Add all jobs from initial jobFile if provided
	addJobsFile(jobsFile);
	setNetworkInterface(host, port);
}

JobQueue::~JobQueue()
{
}

void JobQueue::setNetworkInterface(std::string host, int port)
{
	hostName = host;
	portNumber = port;
}

void JobQueue::addJobsFile(std::string filename)
{
	std::ifstream file(filename);
	std::string line;
	while (std::getline(file, line))
		jobs.push(line);
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
