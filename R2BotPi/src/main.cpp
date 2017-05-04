#include "Global.h"

#include <vector>
#include <iostream>
#include <thread>
#include <unordered_map>
#include <cmath>
#include <chrono>

#include "Controller.h"
#include "Job.h"
#include "JobHandler.h"
#include "Sensor.h"

#include "Controller/UDPClientController.h"
#include "Controller/MotorController.h"
#include "Controller/ManualInputsHandler.h"
#include "JobHandler/ForwardHandler.h"
#include "JobHandler/R2Server.h"
#include "JobHandler/SafetyHandler.h"
#include "Sensor/UDPServerSensor.h"
#include "Sensor/UltrasoundSensor.h"

/** Initializes sensors */
smap<ptr<Sensor>> initializeSensors(smap<string>& args) {
	printf("Initialize sensors\n");

	smap<ptr<Sensor>> sensors;
	if (args.find("udp-server-ip") != args.end() && args.find("udp-server-port") != args.end()) {
		sensors["UDP SERVER"] = std::make_shared<UDPServerSensor>(args["udp-server-ip"], atoi(args["udp-server-port"].c_str()));
	}
	else {
		sensors["UDP SERVER"] = std::make_shared<UDPServerSensor>("0.0.0.0", 9000);
	}

	if (args.find("ultrasound-com-port") != args.end()) {
		sensors["ULTRASOUND"] = std::make_shared<UltrasoundSensor>(args["ultrasound-com-port"], B9600);
	}
	else {
		std::cout << "No ultrasound port specified." << std::endl;
	}

	return sensors;
}

/** Initializes controllers */
smap<ptr<Controller>> initializeControllers(smap<string>& args) {
	printf("Initialize controllers\n");

	smap<ptr<Controller>> controllers;
	if (args.find("udp-nuc-ip") != args.end() && args.find("udp-nuc-port") != args.end()) {
		controllers["UDP NUC"] = std::make_shared<UDPClientController>(args["udp-nuc-ip"], atoi(args["udp-nuc-port"].c_str()));
	}
	else {
		std::cout << "No UDP NUC ip or port specified." << std::endl;
	}

	if (args.find("motor-com-port") != args.end()) {
		controllers["MOTOR"] = std::make_shared<MotorController>(args["motor-com-port"], B9600);
	}
	else if (args.find("use-fake-motor") != args.end()) {
		controllers["MOTOR"] = std::make_shared<FakeMotorController>();
	}
	else {
		std::cout << "No motor port specified." << std::endl;
	}

	return controllers;
}

/** Initialize a list of jobs */
deque<Job> initializeJobs(smap<string>& args) {
	printf("Initialize jobs\n");

	deque<Job> jobs;
	jobs.push_back(Job("manual-inputs"));
	return jobs;
}

/** Initialize background jobs */
deque<JobHandler> initializeBackgroundJobs(smap<string>& args, smap<ptr<Sensor>>& sensors, smap<ptr<Controller>>& controllers) {
	printf("Initializing background jobs\n");

	deque<JobHandler> jobs;
	jobs.push_back(SafetyHandler());

	smap<ptr<Controller>> routes;
	if (controllers.find("UDP NUC") != controllers.end()) {
		routes["PICAMERA"] = controllers["UDP NUC"];
	}
	jobs.push_back(ForwardHandler(routes));
	return jobs;
}

int main(int argc, char *argv[]) {
	smap<string> args = parseArguments(argc, argv);
	/** Initialization */
	smap<ptr<Sensor>> sensors = initializeSensors(args);
	smap<ptr<Controller>> controllers = initializeControllers(args);
	deque<Job> jobQueue = initializeJobs(args);
	ptr<JobHandler> currentJob;
	deque<JobHandler> bgJobs = initializeBackgroundJobs(args, sensors, controllers);

	//TODO: method arg should be sql command stored in a char

	/** Main execution loop */
	while (1) {
		// Collect data from sensors
		printf("Collecting data from sensors\n");
		SensorData data;
		for (auto itr : sensors) {
			ptr<Sensor> sensor = itr.second;
			sensor->fillData(data);
		}

		// Get the next job in the queue
		if (!currentJob) {
			if (jobQueue.size() > 0) {
				Job nextJob = jobQueue.front();
				jobQueue.pop_front();
				currentJob = JobHandler::GetJobHandler(nextJob.getHandler());
			}
		}

		// Run the current job
		printf("Executing the current job\n");
		ControllerData outputs;
		if (currentJob) {
			currentJob->execute(jobQueue, data, outputs);
		}

		// Run background jobs
		printf("Executing background jobs\n");
		for (auto itr : bgJobs) {
			itr.execute(jobQueue, data, outputs);
		}

		// Send output data to controllers
		printf("Sending outputs to controllers\n");
		for (auto itr : controllers) {
			ptr<Controller> controller = itr.second;
			std::cout << controller->getName() << std::endl;
			controller->sendData(outputs);
		}

		// Sleep
		Sleep(20);
	}
}
