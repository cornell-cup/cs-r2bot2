#include "UDPSocketServer.h"
#include "ManualInputs.h"
#include <vector>
using namespace std;

std::queue<string>* ManualInputs::getCommands() {
	return commands;
}

ManualInputs::ManualInputs(std::string host, int port) {
	commandListener = new UDPSocketServer(host, port);
	commands = new queue<string>();
}

void ManualInputs::receiveManualInputs() {
	(*commandListener).server([this](char * buffer, unsigned int buffer_len) {
		std::string manualCommand = std::string(buffer);
		(*commands).push(manualCommand);
	});
}