#include "Job.h"

Job::Job(std::string command): handler() {
}

Job::~Job() {
}

std::string Job::getHandler() {
	return handler;
}