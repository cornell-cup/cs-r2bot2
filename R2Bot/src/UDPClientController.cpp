#include "Controller/UDPClientController.h"
#include "Data/ForwardData.h"

#include <string>

using std::string;

UDPClientController::UDPClientController(string destination, string host, int port) : Controller("UDP Client"), destination(destination), client(std::make_shared<UDPSocketClient>(host, port)) {
	printf("UDP Client connected to host %s and port %d\n", host.c_str(), port);
}

UDPClientController::~UDPClientController() {
}

bool UDPClientController::ping() {
	return client->isConnected() == 1;
}

void UDPClientController::sendData(ControllerData & data) {
	if (client->isConnected()) {
		auto result = data.find("FORWARD");
		if (result == data.end()) {
			return;
		}
		ptr<ForwardData> forward = std::static_pointer_cast<ForwardData>(result->second);
		auto fdata = forward->data.find(destination);
		if (fdata == forward->data.end()) {
			return;
		}
		for (auto itr : fdata->second) {
			std::vector<uint8_t> output;
			R2Protocol::encode(itr, output);
			client->write((const char *)output.data(), (unsigned int)output.size());
		}
	}
}