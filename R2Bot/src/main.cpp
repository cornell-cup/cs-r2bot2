#include "Controller.h"
#include "JobHandler.h"
#include "JobQueue.h"
#include "R2Server.h"
#include "Sensor.h"
#include <vector>
#include <iostream>
#include "KinectSensor.h"
#include <thread>
#include <unordered_map>

std::unordered_map<std::string, Sensor*> sensors;
std::vector<Controller> controllers;
std::vector<JobHandler> jobHandlers;
JobQueue jobQueue;

/** Initializes a Sensor object for each sensor */
void initializeSensors() {
	sensors["KinectSensor"] = new KinectSensor("Kinect Sensor");
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

/** Gets all sensor data in current time step */
void getSensorData() {
	for (auto i : sensors) {
		i.second->getSensorData();
	}
}

int main(int argc, char *argv[]) {
	std::string host = "";
	int port = -1;

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

	/** Create web server for displying image stream */
	char* server_args[] = {"R2Server", "--docroot", ".",
		"--http-address", "0.0.0.0", "--http-port", "8080"};
	KinectSensor *kSensor = static_cast<KinectSensor *>(sensors["KinectSensor"]);
	std::thread serverThread(R2Server::run,
		kSensor->imageWidth,
		kSensor->imageHeight,
		7, server_args);
	std::cout << "\nStarted Server thread\n";

	/** Main execution loop */
	while (1) {
		std::string job = jobQueue.getJob();
		if (job != "") {
			JobHandler *handler = getHandlerByJob(job);
			handler->execute(job);
		}
		getSensorData();
		// Update server image. TODO: maybe move this?
		if (R2Server::k != nullptr) {
			kSensor->imageMutex->lock();
			R2Server::k->setImage(kSensor->jpgImage,
				kSensor->jpgSize);
			kSensor->imageMutex->unlock();
		}
		//break;
		// TODO: Complete this function
	}
}
