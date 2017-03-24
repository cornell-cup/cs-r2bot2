#include "Controller/UDPClientController.h"

UDPClientController::UDPClientController(string host, int port) : Controller("UDP Client"), client(std::make_shared<UDPSocketClient>(host, port)) {
}

UDPClientController::~UDPClientController() {
}

bool UDPClientController::ping() {
	return client->isConnected() == 1;
}

void UDPClientController::sendData(std::string data) {
	if (client->isConnected()) {
		client->write(data.data(), data.length());
	}
}