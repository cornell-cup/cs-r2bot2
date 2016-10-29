#include "UDPSocketServer.h"
#include "ManualInputs.h"
#include "Controller.h"
#include "JobHandler.h"
#include "JobQueue.h"
#include "Sensor.h"
#include <vector>
#include <iostream>
#include "KinectSensor.h"
#include <cmath>  

std::vector<Sensor*> sensors;
std::vector<Controller> controllers;
std::vector<JobHandler> jobHandlers;
JobQueue jobQueue;
ManualInputs *manualInputsHandler;

/** Initializes a Sensor object for each sensor */
void initializeSensors() {
	sensors.push_back(new KinectSensor("Kinect Sensor"));
}

/** Initializes the manual inputs handler by establishing the UDP socket connection to receive data */
void initializeManualInputsHandler(std::string host, int port) {
	WSADATA data;
	WSAStartup(MAKEWORD(2, 2), &data);
	manualInputsHandler = new ManualInputs(host, port);
    (*manualInputsHandler).receiveManualInputs();
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
	for (int i = 0; i < sensors.size(); i++) {
		(*sensors[i]).getSensorData();
	}
}

/** Utility function for splitting a string based on a delimitting character */
void split(const std::string& s, char c, std::vector<std::string>& v) {
	std::string::size_type i = 0;
	std::string::size_type j = s.find(c);

	while (j != std::string::npos) {
		v.push_back(s.substr(i, j - i));
		i = ++j;
		j = s.find(c, j);
		if (j == std::string::npos)
			v.push_back(s.substr(i, s.length()));
	}
}

/** Pulls data from the manual inputs queue and performs the associated action */
void processManualData() {
	std::queue<std::string> *taskQueue = (*manualInputsHandler).getCommands();
	if ((*taskQueue).size() > 0) {
		std::string manualCommand = (*taskQueue).front();
		(*taskQueue).pop();
		if (manualCommand.find("XBOX Kinect") == 0) {
			std::vector<std::string> v;
			std::string s = manualCommand.substr(12, 100);
			split(s, ' ', v);
			for (int i = 0; i < v.size(); ++i) {
				std::string tmp = v[i];
				char number[1024];
				strcpy_s(number, tmp.c_str());
			}
			int first = std::stoi(v[0]);
			int second = std::stoi(v[1]);
			if ((std::abs(first - 80.0) + std::abs(second - 80.0)) <= 10.0) {
				KinectSensor *currentSensor = dynamic_cast<KinectSensor*>((sensors[0]));
				currentSensor->sensor->NuiCameraElevationSetAngle(27);
			}
			else if ((std::abs(first + 80) + std::abs(second + 80.0)) <= 10.0) {
				KinectSensor *currentSensor = dynamic_cast<KinectSensor*>((sensors[0]));
				currentSensor->sensor->NuiCameraElevationSetAngle(-27);
			}
		}
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
	initializeManualInputsHandler("192.168.4.170", 9020);
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
		getSensorData();
		processManualData();
		// TODO: Complete this function
	}
}