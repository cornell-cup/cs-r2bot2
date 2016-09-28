#include "Controller.h"
#include "JobHandler.h"
#include "JobQueue.h"
#include "Sensor.h"
#include <list>

std::list<Sensor> sensors;
std::list<Controller> controllers;
std::list<JobHandler> jobHandlers;
JobQueue jobQueue;

/** Initializes a Sensor object for each sensor */
void initializeSensors() {
	// "TODO: initializeSensors"
}

/** Initializes a Controller object for each controller */
void initializeControllers() {
	// "TODO: initializeControllers"
}

/** Initializes a JobHandler object for each job type */
void initializeJobHandlers() {
	// "TODO: initializeJobHandlers"
}

/** Initializes the JobQueue object */
void initializeJobQueue() {
	// "TODO: initializeJobQueue"
}

/** Returns the JobHandler object suited to deal with the given job */
JobHandler getHandlerByJob(std::string job) {
	// "TODO: initializeJobQueue"
}

int main(int argc, char *argv[]) {
	initializeSensors();
	initializeControllers();
	initializeJobHandlers();
	initializeJobQueue();

	/** Main execution loop */
	while (1) {
		std::string job;
		if (jobQueue.getJob(&job)) {
			JobHandler handler = getHandlerByJob(job);
			handler.execute(job);
		}

		// TODO: Complete this function
	}
}