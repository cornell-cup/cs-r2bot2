#include "Controller/UDPClientController.h"

UDPClientController::UDPClientController(string host, int port) : Controller("UDP Client"), client(new UDPSocketClient(host, port)) {
}

UDPClientController::~UDPClientController() {
}

bool UDPClientController::ping() {
	return false;
}

void UDPClientController::sendData(std::string data) {
}