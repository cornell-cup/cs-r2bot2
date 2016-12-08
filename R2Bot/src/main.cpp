#include "UDPSocketServer.h"
#include "ManualInputs.h"
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
#include <cmath>
#include <chrono>

std::unordered_map<std::string, Sensor*> sensors;
std::vector<Controller> controllers;
std::vector<JobHandler> jobHandlers;
JobQueue jobQueue;
ManualInputs *manualInputsHandler;
int lastKinectTiltAngle = 0;

/** Initializes a Sensor object for each sensor */
void initializeSensors() {
	sensors["KinectSensor"] = new KinectSensor("Kinect Sensor");
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
	for (auto i : sensors) {
		i.second->getSensorData();
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
	while (TRUE) {
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
					KinectSensor *currentSensor = dynamic_cast<KinectSensor*>(sensors["KinectSensor"]);
					if (lastKinectTiltAngle == -27) {
						currentSensor->sensor->NuiCameraElevationSetAngle(0);
						lastKinectTiltAngle = 0;
						std::this_thread::sleep_for(std::chrono::seconds(1));
					}
					else if (lastKinectTiltAngle == 0) {
						currentSensor->sensor->NuiCameraElevationSetAngle(27);
						lastKinectTiltAngle = 27;
						std::this_thread::sleep_for(std::chrono::seconds(1));
					}
				}
				else if ((std::abs(first + 80) + std::abs(second + 80.0)) <= 10.0) {
					KinectSensor *currentSensor = dynamic_cast<KinectSensor*>(sensors["KinectSensor"]);
					if (lastKinectTiltAngle == 27) {
						currentSensor->sensor->NuiCameraElevationSetAngle(0);
						lastKinectTiltAngle = 0;
						std::this_thread::sleep_for(std::chrono::seconds(1));
					}
					else if (lastKinectTiltAngle == 0) {
						currentSensor->sensor->NuiCameraElevationSetAngle(-27);
						lastKinectTiltAngle = -27;
						std::this_thread::sleep_for(std::chrono::seconds(1));
					}
				}
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

	/** Create web server for displying image stream */
	char* server_args[] = {"R2Server", "--docroot", ".",
		"--http-address", "0.0.0.0", "--http-port", "8080"};
	KinectSensor *kSensor = static_cast<KinectSensor *>(sensors["KinectSensor"]);
	std::thread serverThread(R2Server::run,
		kSensor->imageWidth,
		kSensor->imageHeight,
		7, server_args);
	std::cout << "\nStarted Server thread\n";

	/** Create manual input thread */
	std::thread manualInputThread(processManualData);
	std::cout << "\nStarted Manual Input Thread\n";

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
