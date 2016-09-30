#include "JobQueue.h"

JobQueue::JobQueue() : hostName(""), portNumber(0)
{
}

JobQueue::JobQueue(std::string host, int port) : hostName(host), portNumber(port)
{
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
