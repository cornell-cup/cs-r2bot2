#include "JobHandler/R2Server.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include "crow_all.h"
#include <regex>

#include "Data/GamepadData.h"
#include "Data/HeadData.h"
#include "Data/DrawerCommand.h"
#include "Data/RFIDData.h"
#include "Data/IMUData.h"
#include "Data/LidarData.h"
#include "Data/HeadData.h"
#include "Data/UltrasoundData.h"
#include <string>
#include <vector>

// Read in a file and return a string containing the byte array input
static string readIn(string fileName) {
	std::cout << fileName << std::endl;
	std::ifstream file(fileName, std::ios::in | std::ios::binary | std::ios::ate);
	
	if (!file.is_open())
		throw std::runtime_error("couldn't open");

	vector<char> fileContents;
	fileContents.resize(file.tellg());


	file.seekg(0, std::ios::beg);
	if (!file.read(&fileContents[0], fileContents.size())) {
		throw std::runtime_error("failed to read");
	}
	return string(fileContents.data(), fileContents.size());
}

static string MimeTypeFromString(const string& str) {
	static smap<string> MIME_TYPES{
		{ ".html", "text/html" },
		{ ".htm", "text/html" },
		{ ".css", "text/css" },
		{ ".js", "text/javascript" },
		{ ".txt", "text/plain" },
		{ ".jpg", "image/jpeg" },
		{ ".png", "image/png" },
		{ ".bmp", "image/bmp" },
		{ ".mp3", "audio/mpeg" },
		{ ".mp4", "video/mp4" },
		{ ".pdf", "application/pdf" },
		{ "*", "application/octet-stream" }
	};

	auto mime = MIME_TYPES.find(str);
	if (mime != MIME_TYPES.end()) {
		return mime->second;
	}
	else {
		return MIME_TYPES["*"];
	}
}

bool R2Server::registered = JobHandler::RegisterJobHandler("r2-server", [](string command) {
	return std::static_pointer_cast<JobHandler>(std::make_shared<R2Server>(18080));
});

string ultrasoundInput;
string drawerC;
string drawerRFID;
string toolInv;
string userList;
string imuDirection;
R2Server::R2Server(int port):JobHandler(), Sensor() {
	CROW_ROUTE(app, "/wsc")
		.websocket()
		.onopen([&](crow::websocket::connection& conn) {
		CROW_LOG_INFO << "new websocket connection";
		std::lock_guard<std::mutex> _(mtx);
		users.insert(&conn);
	})
		.onclose([&](crow::websocket::connection& conn, const std::string& reason) {
		CROW_LOG_INFO << "websocket connection closed: " << reason;
		std::lock_guard<std::mutex> _(mtx);
		users.erase(&conn);
	})
		.onmessage([&](crow::websocket::connection& /*conn*/, const std::string& data, bool is_binary) {
		std::lock_guard<std::mutex> _(mtx);
		for (auto u : users) {
			if (is_binary) {
				u->send_binary("hello");
			}
			else {
				u->send_binary(readIn("../R2Bot/templates/ccrt-logo.png"));
			}
		}
	});

	CROW_ROUTE(app, "/wsd")
		.websocket()
		.onopen([&](crow::websocket::connection& conn) {
		CROW_LOG_INFO << "new websocket connection";
		std::lock_guard<std::mutex> _(mtx);
		users.insert(&conn);
	})
		.onclose([&](crow::websocket::connection& conn, const std::string& reason) {
		CROW_LOG_INFO << "websocket connection closed: " << reason;
		std::lock_guard<std::mutex> _(mtx);
		users.erase(&conn);
	})
		.onmessage([&](crow::websocket::connection& /*conn*/, const std::string& data, bool is_binary) {
		std::lock_guard<std::mutex> _(mtx);

		for (auto u : users){
			if (is_binary) {
				u->send_binary("hi");
			}
			else {
				if (drawerC.length() != 0) {
					u->send_binary("2" + drawerC);
				}
				if (drawerRFID.length() != 0) {
					u->send_binary("3" + drawerRFID);
				}
				if (toolInv.length() != 0) {
					u->send_binary("T"+toolInv);
				}
				if (userList.length() != 0) {
					u->send_binary("U" + userList);
				}
			}
		}
	});

	CROW_ROUTE(app, "/wsm")
		.websocket()
		.onopen([&](crow::websocket::connection& conn) {
		CROW_LOG_INFO << "new websocket connection";
		std::lock_guard<std::mutex> _(mtx);
		users.insert(&conn);
	})
		.onclose([&](crow::websocket::connection& conn, const std::string& reason) {
		CROW_LOG_INFO << "websocket connection closed: " << reason;
		std::lock_guard<std::mutex> _(mtx);
		users.erase(&conn);
	})
		.onmessage([&](crow::websocket::connection& /*conn*/, const std::string& data, bool is_binary) {
		std::lock_guard<std::mutex> _(mtx);
		manualInput = data;

		for (auto u : users) {
			if (is_binary) {
				u->send_binary("6");
				std::cout << data << std::endl;
			}
			else {
				if (ultrasoundInput.length() != 0) {
					std::cout << ultrasoundInput << std::endl;
					u->send_binary(ultrasoundInput);
					ultrasoundInput = "";
				}
				if (imuDirection.length() != 0) {
					u->send_binary("I" + imuDirection);
				}
			}
		}
	});

	CROW_ROUTE(app, "/wsh")
		.websocket()
		.onopen([&](crow::websocket::connection& conn) {
		CROW_LOG_INFO << "new websocket connection";
		std::lock_guard<std::mutex> _(mtx);
		users.insert(&conn);
	})
		.onclose([&](crow::websocket::connection& conn, const std::string& reason) {
		CROW_LOG_INFO << "websocket connection closed: " << reason;
		std::lock_guard<std::mutex> _(mtx);
		users.erase(&conn);
	})
		.onmessage([&](crow::websocket::connection& /*conn*/, const std::string& data, bool is_binary) {
		std::lock_guard<std::mutex> _(mtx);

		homeInput = data;

		for (auto u : users) {
			if (is_binary) {
				u->send_binary("data recieved");
			}
			else {

			}
		}
	});

	CROW_ROUTE(app, "/<string>") ([](const crow::request& req, crow::response& res, std::string str) {
		size_t index = str.rfind('.');
		string contentType = (index == str.size()) ? "application/octet-stream" : MimeTypeFromString(str.substr(index));
		res.add_header("Content-Type", contentType);
		res.write(readIn("../R2Bot/templates/" + str));
		res.end();
	});

	std::thread([&, port]() {
		printf("R2 server listening on port %d\n", port);
		app.port(port).run();
	}).detach();
}

R2Server::~R2Server() {
}

string R2Server::getName() {
	return "R2 Server";
}

bool R2Server::ping() {
	return true;
}

void R2Server::fillData(SensorData& sensorData) {
	if (manualInput.find(" ") >= 0) {
		std::istringstream ss(manualInput);
		ptr<GamepadData> data = std::make_shared<GamepadData>();
		if (ss >> data->x >> data->y) {
			sensorData["GAMEPAD"] = data;
		}
	}
	if (manualInput.length() > 0){
		ptr<HeadData> data = std::make_shared<HeadData>();
		if (manualInput.substr(0, 1) == "P") {
			data->angle = std::stoi(manualInput.substr(2, manualInput.length() - 2));
			sensorData["HEAD"] = data;
		}
		else if (manualInput.substr(0, 1) == "L" || manualInput.substr(0,1) == "R") {
			data->time = std::stoi(manualInput.substr(2, manualInput.length() - 2));
			sensorData["HEAD"] = data;
		}
		else if (manualInput == "O" || manualInput == "C") {
			sensorData["FLAP"] = std::make_shared<string>(manualInput);
		}
		else if (manualInput == "Get Head Angle") {
			sensorData["HEAD"] = std::make_shared<string>("G");
		}
		else if (manualInput.substr(0,1) == "S") {
			sensorData["SOUND"] = std::make_shared<string>(manualInput.substr(1));
		}
	}

}

void R2Server::execute(deque<Job>& jobs, SensorData& data, ControllerData& outputs) {
	//std::cout << "EXECUTING" << std::endl;
	auto result = data.find("ULTRASOUNDB");
	if (result != data.end()) {
		for (int i = 1; i <= 7; i++) {
			ultrasoundInput += "U" + std::to_string(i) + "SENSOR";
			string inches = std::to_string(std::static_pointer_cast<UltrasoundData>(result->second)->distance[i-1]);
			ultrasoundInput += string(",");
			ultrasoundInput += inches;
			ultrasoundInput += "|";
		}
	}

	result = data.find("ULTRASOUNDF");
	if (result != data.end()) {
		for (int i = 8; i <= 14; i++) {
			ultrasoundInput += "U" + std::to_string(i) + "SENSOR";
			string inches = std::to_string(std::static_pointer_cast<UltrasoundData>(result->second)->distance[i - 8]);
			std::cout << inches << "\n";
			ultrasoundInput += string(",");
			ultrasoundInput += inches;
			ultrasoundInput += "|";
		}
	}

	result = outputs.find("DRAWERCOMMAND");
	if (result != outputs.end()) {
		drawerC = std::static_pointer_cast<DrawerCommand>(result->second)->state;
	}
	result = data.find("RFID");
	if (result != data.end()) {
		drawerRFID = std::static_pointer_cast<RFIDData>(result->second)->ID;
	}
	result = outputs.find("TOOLS");
	if (result != outputs.end()) {
		ptr<string> t = std::static_pointer_cast<string>(result->second);
		toolInv = *t;
	}
	result = outputs.find("USERS");
	if (result != outputs.end()) {
		ptr<string> u = std::static_pointer_cast<string>(result->second);
		userList = *u;
	}
	result = data.find("IMU");
	if (result != data.end()) {
		imuDirection = std::to_string(std::static_pointer_cast<IMUData>(result->second)->xDirection);
	}
}