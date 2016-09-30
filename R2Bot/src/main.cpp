#include "Controller.h"
#include "JobHandler.h"
#include "JobQueue.h"
#include "Sensor.h"
#include <vector>
#include <iostream>

#define DEFAULT_HOST "localhost"
#define DEFAULT_PORT 5555

std::vector<Sensor> sensors;
std::vector<Controller> controllers;
std::vector<JobHandler> jobHandlers;
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
void initializeJobQueue(std::string jobsFile, std::string host, int port) {
	jobQueue = JobQueue(jobsFile, host, port);

	// "TODO: initializeJobQueue"
}

/** Returns the JobHandler object suited to deal with the given job */
JobHandler getHandlerByJob(std::string job) {
	// "TODO: initializeJobQueue"
}

int main(int argc, char *argv[]) {
	/** Initialize variables */
	std::string host = DEFAULT_HOST;		// TODO: Changed through argv
	int port = DEFAULT_PORT;				// TODO: Changed through argv

	/** Handle arguments */
	std::string jobsFile;
	for (int i = 0; i < argc; i++) {
		if (argv[i] == "--jobs-file") {
			// Set initial jobs file for job_queue initialization
			if (i + 1 < argc) {
				i++;
				jobsFile = argv[i];
			}
			else {
				std::cout << "jobs-file flag requires an argument";
			}
		}
	}

	/** Initialization */
	initializeSensors();
	initializeControllers();
	initializeJobHandlers();
	initializeJobQueue(jobsFile, host, port);

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