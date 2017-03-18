#include "UDPSocketServer.h"
#include "ManualInputs.h"
#include "Controller.h"
#include "JobHandler.h"
#include "JobQueue.h"
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