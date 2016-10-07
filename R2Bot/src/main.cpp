#include "Controller.h"
#include "JobHandler.h"
#include "JobQueue.h"
#include "Sensor.h"
#include <vector>
#include <iostream>
#include "KinectSensor.h"

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
JobHandler *getHandlerByJob(std::string job) {
	// "TODO: initializeJobQueue"
	return nullptr;
}

int main(int argc, char *argv[]) {
	std::string host = "";
	int port = -1;

	Kinect_main();



	/** Handle arguments */
	for (int i = 0; i < argc; i++) {
		std::string arg = std::string(argv[i]);
		if (arg == "--jobs-file") {
			// Set initial jobs file for job_queue initialization
			if (++i < argc) {
				jobQueue.addJobsFile(argv[i]);
			}
			else {
				std::cout << "jobs-file flag requires an argument";
			}
		}
		else if (arg == "--host") {
			// Set network host for job_queue initialization
			if (++i < argc)
				host = arg;
			else
				std::cout << "host flag requires an argument";
		}
		else if (arg == "--port") {
			// Set network port for job_queue initialization
			if (++i < argc)
				port = stoi(arg);
			else
				std::cout << "port flag requires an argument";
		}
		else {
			// Unrecognized flag
			std::cout << "Unrecognized flag " << arg;
		}
	}

	if (host != "" && port >= 0)
		jobQueue.setNetworkInterface(host, port);

	/** Initialization */
	initializeSensors();
	initializeControllers();
	initializeJobHandlers();
	initializeJobQueue();

	/** Main execution loop */
	while (1) {
		std::string job = jobQueue.getJob();
		if (job != "") {
			JobHandler *handler = getHandlerByJob(job);
			handler->execute(job);
		}

		// TODO: Complete this function
	}
}