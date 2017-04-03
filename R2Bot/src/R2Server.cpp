#include "JobHandler/R2Server.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include "crow_all.h"

// Read in a file and return a string containing the byte array input
static string readIn(string fileName) {
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

R2Server::R2Server(int port) {
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
		for (auto u : users)
			if (is_binary) {
				u->send_binary("hello");
			}
			else {
				u->send_binary(readIn("templates/ccrt-logo.png"));
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

		for (auto u : users)
			if (is_binary) {
				u->send_binary("data recieved");
				std::cout << data << std::endl;
			}
			else {
				u->send_binary("data recieved");
				std::cout << data << std::endl;
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

		std::string identifier = data.substr(0, 1);
		std::string text = data.substr(1);
		homeInput = text;

		if (identifier == "0") {

		}
		else if (identifier == "1") {

		}
		else if (identifier == "2") {
			//homeInput = 
		}
		else if (identifier == "3") {

		}
		else if (identifier == "4") {
			std::cout << "test";
			std::ofstream headFlap;
			headFlap.open("sensorIO/headFlap.txt", std::ios_base::app);
			headFlap << homeInput + "\n";
			headFlap.close();
		}

		for (auto u : users) {
			if (is_binary) {
				u->send_binary("data recieved");
				//std::cout << data << std::endl;
			}
			else {
				u->send_binary("data recieved");
				//std::cout << data << std::endl;
			}
		}
	});

	CROW_ROUTE(app, "/<string>") ([](const crow::request& req, crow::response& res, std::string str) {
		size_t index = str.rfind('.');
		string contentType = (index == str.size()) ? "application/octet-stream" : MimeTypeFromString(str.substr(index));
		res.add_header("Content-Type", contentType);
		res.write(readIn("templates/" + str));
		res.end();
	});

	std::thread([&, port]() {
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

void R2Server::getData(smap<ptr<SensorData>>& sensorData) {
	if (manualInput.find(" ") >= 0) {
		vector<uint8_t> data(manualInput.begin(), manualInput.end());
		sensorData["gamepad"] = SensorData::DecodeSensorData("gamepad", data);
	}
}

void R2Server::execute(deque<Job>& jobs, smap<ptr<SensorData>>& data, smap<string>& outputs) {

}