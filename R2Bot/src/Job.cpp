#include "Job.h"

Job::Job(std::string command) {
	handler = command;
}

Job::~Job() {
}

std::string Job::getHandler() {
	return handler;
}