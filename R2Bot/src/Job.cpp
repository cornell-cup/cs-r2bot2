#include "Job.h"

Job::Job(std::string handler): handler(handler) {
}

Job::~Job() {
}

std::string Job::getHandler() {
	return handler;
}