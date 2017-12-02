#include "Global.h"

#include <vector>
#include <iostream>
#include <deque>
#include <string>
#include <thread>
#include <unordered_map>
#include <cmath>
#include <chrono>

#include "Controller.h"
#include "Job.h"
#include "JobHandler.h"
#include "Sensor.h"

#include "Controller/UDPClientController.h"
#include "Controller/FakeMotorController.h"
#include "Controller/MotorController.h"
#include "Controller/SoundController.h"
#include "JobHandler/ManualInputsHandler.h"
#include "JobHandler/ForwardHandler.h"
#include "JobHandler/R2Server.h"
#include "JobHandler/SafetyHandler.h"
#include "JobHandler/HeadHandler.h"
#include "JobHandler/HeadFlapHandler.h"
#include "JobHandler/SoundHandler.h"
#include "Sensor/UDPServerSensor.h"
#include "Sensor/UltrasoundSensor.h"
#include "JobHandler/RoamerHandler.h"

using std::deque;
using std::string;
using std::vector;

/** Initializes sensors */
smap<ptr<Sensor>> initializeSensors(smap<string>& args) {
#ifdef DEBUG_PRINTS
	printf("*** Initialize sensors\n");
#endif

	smap<ptr<Sensor>> sensors;
	if (args.find("udp-server-ip") != args.end() && args.find("udp-server-port") != args.end()) {
		sensors["UDP SERVER"] = std::make_shared<UDPServerSensor>(args["udp-server-ip"], atoi(args["udp-server-port"].c_str()));
	}
	else {
		sensors["UDP SERVER"] = std::make_shared<UDPServerSensor>("0.0.0.0", 9000);
	}

	if (args.find("ultrasound-com-port") != args.end() || args.find("ultrasound-com-port2") != args.end()) {
		sensors["ULTRASOUND"] = std::make_shared<UltrasoundSensor>(args["ultrasound-com-port"], B9600, args["ultrasound-com-port2"], B9600);
	}
	else {
		std::cout << "No ultrasound port specified." << std::endl;
	}

	return sensors;
}

/** Initializes controllers */
smap<ptr<Controller>> initializeControllers(smap<string>& args) {
#ifdef DEBUG_PRINTS
	printf("*** Initialize controllers\n");
#endif

	smap<ptr<Controller>> controllers;
	if (args.find("udp-nuc-ip") != args.end() && args.find("udp-nuc-port") != args.end()) {
		controllers["UDP NUC"] = std::make_shared<UDPClientController>("UDP NUC", args["udp-nuc-ip"], atoi(args["udp-nuc-port"].c_str()));
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

	controllers["SOUND"] = std::make_shared<SoundController>();

	return controllers;
}

/** Initialize a list of jobs */
deque<Job> initializeJobs(smap<string>& args) {
#ifdef DEBUG_PRINTS
	printf("*** Initialize jobs\n");
#endif

	deque<Job> jobs;
	jobs.push_back(Job("roamer"));
	//jobs.push_back(Job("manual-inputs"));
	return jobs;
}

/** Initialize background jobs */
deque<ptr<JobHandler>> initializeBackgroundJobs(smap<string>& args, smap<ptr<Sensor>>& sensors, smap<ptr<Controller>>& controllers) {
#ifdef DEBUG_PRINT
	printf("*** Initializing background jobs\n");
#endif

	deque<ptr<JobHandler>> jobs;
	if (args.find("disable-safety") != args.end()) {
		printf("WARNING: DISABLING MOTOR SAFETY\n");
	}
	else {
		jobs.push_back(std::static_pointer_cast<JobHandler>(std::make_shared<SafetyHandler>()));
	}

	smap<ptr<Controller>> routes;
	if (controllers.find("UDP NUC") != controllers.end()) {
		routes["PICAMERA"] = controllers["UDP NUC"];
	}
	jobs.push_back(std::static_pointer_cast<JobHandler>(std::make_shared<SoundHandler>()));
	jobs.push_back(std::static_pointer_cast<JobHandler>(std::make_shared<HeadHandler>()));
	jobs.push_back(std::static_pointer_cast<JobHandler>(std::make_shared<HeadFlapHandler>()));
	return jobs;
}

int main(int argc, char *argv[]) {
	R2Bot::DEVICE_NAME = "PI";
	smap<string> args = parseArguments(argc, argv);
	/** Initialization */
	smap<ptr<Sensor>> sensors = initializeSensors(args);
	smap<ptr<Controller>> controllers = initializeControllers(args);
	deque<Job> jobQueue = initializeJobs(args);
	ptr<JobHandler> currentJob;
	deque<ptr<JobHandler>> bgJobs = initializeBackgroundJobs(args, sensors, controllers);

	//TODO: method arg should be sql command stored in a char

	/** Main execution loop */
	while (1) {
		// Collect data from sensors
#ifdef DEBUG_PRINTS
		printf("** Collecting data from sensors\n");
#endif
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
		ControllerData outputs;
		if (currentJob) {
#ifdef DEBUG_PRINTS
			printf("** Executing the current job\n");
#endif
			currentJob->execute(jobQueue, data, outputs);
		}
#ifdef DEBUG_PRINTS
		else {
			printf("** No current job to execute\n");
		}
#endif

		// Run background jobs
#ifdef DEBUG_PRINTS
		printf("** Executing background jobs\n");
#endif
		for (auto itr : bgJobs) {
			(*itr).execute(jobQueue, data, outputs);
		}

		// Send output data to controllers
#ifdef DEBUG_PRINTS
		printf("** Sending outputs to controllers\n");
#endif
		for (auto itr : controllers) {
			ptr<Controller> controller = itr.second;
			controller->sendData(outputs);
		}

		// Sleep
		Sleep(20);
	}
}
