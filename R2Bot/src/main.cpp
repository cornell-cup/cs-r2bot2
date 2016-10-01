#include "Controller.h"
#include "JobHandler.h"
#include "JobQueue.h"
#include "Sensor.h"
#include <vector>
#include <iostream>

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
void initializeJobQueue() {
	// "TODO: initializeJobQueue"
}

/** Returns the JobHandler object suited to deal with the given job */
JobHandler getHandlerByJob(std::string job) {
	// "TODO: initializeJobQueue"
}

int main(int argc, char *argv[]) {
	/** Handle arguments */
	for (int i = 0; i < argc; i++) {
		if (std::string(argv[i]) == "--jobs-file") {
			// Set initial jobs file for job_queue initialization
			if (i + 1 < argc) {
				i++;
				jobQueue.addJobsFile(argv[i]);
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
	initializeJobQueue();

	/** Main execution loop */
	while (1) {
		std::string job = jobQueue.getJob();
		if (job != "") {
			JobHandler handler = getHandlerByJob(job);
			handler.execute(job);
		}

		// TODO: Complete this function
	}
}